import * as THREE from "three";
import $ from "jquery";
import { scene } from "./scene";
import { camera, updateControls } from "./camera";
import { connect, disconnect, sendCommand } from "./websocket_api";
import {
  addEntity,
  updateEntity,
  removeEntity,
  updateAnimations,
  entities
} from "./model";
import { loadScene } from "./sceneLoader";
import { initScheduler } from "./tripScheduler";
import { notify } from "./notifications";

// Define interfaces for package information
interface PackageInfo {
  id: number;
  name: string;
  priority: string;
  position: THREE.Vector3;
  isPickedUp: boolean;
}

// Store package information for priority management
const packages: Map<number, PackageInfo> = new Map();
let packageQueue: PackageInfo[] = [];

const container = $("#scene-container")[0];
const simSpeedSlider = $("#sim-speed");
const stopSimulationButton = $("#stop-simulation")[0];
const addHumanButton = $("#add-human")[0];
const deliveryPopup = $("#delivery-popup");

// Priority management UI elements
const changePriorityButton = $("#change-priority")[0];
const changePriorityInput = $("#change-priority-input")[0];
const changePriorityName = $("#change-priority-name")[0] as HTMLInputElement;
const changePriorityValue = $("#change-priority-value")[0] as HTMLSelectElement;
const changePrioritySubmit = $("#change-priority-submit")[0];
const changePriorityCancel = $("#change-priority-cancel")[0];
const changePriorityError = $("#change-priority-error")[0];
const refreshPackagesButton = $("#refresh-packages")[0];
const priorityQueueList = $("#priority-queue-list")[0];

const sceneFile = "scenes/umn.json";
const clock = new THREE.Clock();

let time = 0.0;
let simSpeed = 1.0;
let renderer = new THREE.WebGLRenderer({ antialias: true });

simSpeedSlider[0].oninput = () => {
  simSpeed = (simSpeedSlider.val() as number) / 10.0;
};

stopSimulationButton.onclick = () => {
  sendCommand("stopSimulation", {});
  disconnect();
};

let humanID = 1;
addHumanButton.onclick = () => {
  sendCommand("CreateEntity", {
    type: "human",
    name: "Human-" + humanID,
    mesh: "assets/model/human.glb",
    position: [700, 290, 400],
    scale: [0.005, 0.005, 0.005],
    rotation: [0, 0, 0, 0],
    direction: [1, 0, 0],
    speed: 10.0,
    radius: 1.0,
    start: 2.0,
    duration: 2.0,
    offset: [0, -0.5, 0],
  });
  humanID += 1;
};

// Update the priority queue list display
function updatePriorityQueueList() {
  // Clear the current list
  priorityQueueList.innerHTML = "";
  
  // Add packages to the queue list
  packageQueue.forEach(pkg => {
    const item = document.createElement("li");
    
    // Show status (in transit or waiting) and whether priority can be changed
    let status = pkg.isPickedUp ? "(In Transit)" : "(Waiting)";
    let priorityChangeInfo = pkg.isPickedUp ? " - Priority locked" : " - Priority can be changed";
    
    item.textContent = `${pkg.name} - ${pkg.priority} ${status}${priorityChangeInfo}`;
    
    // Add appropriate class based on priority and pickup status
    item.classList.add(`queue-item-${pkg.priority.toLowerCase()}`);
    
    // Gray out items that can't be modified
    if (pkg.isPickedUp) {
      item.classList.add('picked-up');
    }
    
    priorityQueueList.appendChild(item);
  });
  
  // Show a message if there are no packages
  if (packageQueue.length === 0) {
    const item = document.createElement("li");
    item.textContent = "No packages in queue";
    priorityQueueList.appendChild(item);
  }
}

// Request package queue data from server
function refreshPackages() {
  // Request the current delivery queue from the server
  sendCommand("GetDeliveryQueue", {});
  changePriorityError.textContent = "Refreshing package list...";
}

// Update package queue locally with current data
function refreshPackagesLocally() {
  // Reset the package queue
  packageQueue = [];
  
  // Add all known packages to the queue
  packages.forEach(pkg => {
    packageQueue.push(pkg);
  });
  
  // Sort by priority level
  packageQueue.sort((a, b) => {
    const getPriorityLevel = (priority: string): number => {
      switch (priority) {
        case "Expedited": return 3;
        case "Standard": return 2;
        case "NoRush": return 1;
        default: return 0;
      }
    };
    return getPriorityLevel(b.priority) - getPriorityLevel(a.priority);
  });
  
  // Update the UI
  updatePriorityQueueList();
  
  changePriorityError.textContent = "";
}

// Priority management UI handlers
changePriorityButton.onclick = () => {
  changePriorityInput.hidden = !changePriorityInput.hidden;
  if (!changePriorityInput.hidden) {
    changePriorityName.value = "";
    refreshPackages();
  }
};

changePrioritySubmit.onclick = () => {
  if (!changePriorityName.value) {
    changePriorityError.textContent = "Please enter a package name";
    return;
  }
  
  const packageName = changePriorityName.value;
  const newPriority = changePriorityValue.value;
  
  // Check if the package is already picked up
  let isPickedUp = false;
  for (const pkg of packageQueue) {
    if (pkg.name === packageName && pkg.isPickedUp) {
      isPickedUp = true;
      break;
    }
  }
  
  if (isPickedUp) {
    changePriorityError.textContent = "Cannot change priority for packages in transit";
    return;
  }
  
  sendCommand("ChangePriority", {
    packageName: packageName,
    priority: newPriority
  });
  
  changePriorityError.textContent = "Priority update sent!";
  setTimeout(() => {
    refreshPackages();
    changePriorityError.textContent = "";
  }, 1000);
};

refreshPackagesButton.onclick = refreshPackages;

changePriorityCancel.onclick = () => {
  changePriorityInput.hidden = true;
  changePriorityError.textContent = "";
};

window.onresize = () => {
  camera.aspect = container.clientWidth / container.clientHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(container.clientWidth, container.clientHeight);
};

initScheduler();

connect().then((socket) => {
  socket.onmessage = (msg) => {
    const data = JSON.parse(msg.data);
    switch (data.event) {
      case "AddEntity":
        addEntity(data.details.id, data.details.details);
        
        // Track packages for priority management
        if (data.details.details && data.details.details.type === "package") {
          const entityName = data.details.details.name;
          const entityId = data.details.id;
          const position = new THREE.Vector3(...data.details.pos);
          
          // Get priority if available
          let priority = "Standard";
          if (data.details.priority) {
            priority = data.details.priority;
          }
          
          // Add to package tracking
          packages.set(entityId, {
            id: entityId,
            name: entityName,
            priority: priority,
            position: position,
            isPickedUp: false
          });
          
          // Refresh UI if the priority management panel is open
          if (!changePriorityInput.hidden) {
            refreshPackagesLocally();
          }
        }
        break;
      case "UpdateEntity":
        updateEntity(data.details.id, data.details);
        
        // Update package position and priority if tracked
        if (packages.has(data.details.id)) {
          const pkg = packages.get(data.details.id)!;
          pkg.position = new THREE.Vector3(...data.details.pos);
          
          // Update priority if provided
          if (data.details.priority) {
            pkg.priority = data.details.priority;
          }
          
          // Update pickup status if provided
          if (data.details.isPickedUp !== undefined) {
            pkg.isPickedUp = data.details.isPickedUp;
          }
          
          // Refresh UI if the priority management panel is open
          if (!changePriorityInput.hidden) {
            refreshPackagesLocally();
          }
        }
        break;
      case "RemoveEntity":
        removeEntity(data.details.id);
        
        // Remove from package tracking if applicable
        if (packages.has(data.details.id)) {
          packages.delete(data.details.id);
          
          // Refresh UI if the priority management panel is open
          if (!changePriorityInput.hidden) {
            refreshPackagesLocally();
          }
        }
        break;
      case "DeliveryQueueInfo":
        // Update package information from server
        if (data.details.packages && Array.isArray(data.details.packages)) {
          data.details.packages.forEach((pkg: any) => {
            if (pkg.id && packages.has(pkg.id)) {
              const existingPkg = packages.get(pkg.id)!;
              existingPkg.priority = pkg.priority || "Standard";
              
              if (pkg.isPickedUp !== undefined) {
                existingPkg.isPickedUp = pkg.isPickedUp;
              }
              
              if (pkg.position && Array.isArray(pkg.position)) {
                existingPkg.position = new THREE.Vector3(...pkg.position);
              }
            } else if (pkg.id) {
              // New package we haven't seen before
              packages.set(pkg.id, {
                id: pkg.id,
                name: pkg.name,
                priority: pkg.priority || "Standard",
                position: pkg.position ? new THREE.Vector3(...pkg.position) : new THREE.Vector3(),
                isPickedUp: pkg.isPickedUp || false
              });
            }
          });
          
          refreshPackagesLocally();
          changePriorityError.textContent = "Package list refreshed from server";
          setTimeout(() => {
            changePriorityError.textContent = "";
          }, 2000);
        }
        break;
      case "DeliveryQueueUpdated":
        // Queue has been updated, refresh our local view
        refreshPackages();
        break;
      case "Notification":
        const message = data.details.message;
        
        // Extract priority information from notifications
        if (message.includes("priority changed to")) {
          const packageName = message.split(" priority changed to ")[0].replace("Package ", "");
          const priority = message.split(" priority changed to ")[1];
          
          // Update our local package data
          for (const [id, pkg] of packages.entries()) {
            if (pkg.name === packageName) {
              pkg.priority = priority;
              break;
            }
          }
          
          // Refresh UI if the priority management panel is open
          if (!changePriorityInput.hidden) {
            refreshPackagesLocally();
          }
        } else if (message.includes("picked up:")) {
          // Extract package name from pickup notification
          const match = message.match(/picked up: (.+?) \(/);
          if (match && match[1]) {
            const packageName = match[1];
            
            // Update our local package data
            for (const [id, pkg] of packages.entries()) {
              if (pkg.name === packageName) {
                pkg.isPickedUp = true;
                break;
              }
            }
            
            // Refresh UI if the priority management panel is open
            if (!changePriorityInput.hidden) {
              refreshPackagesLocally();
            }
          }
        }
        
        // Color-code notifications based on priority
        let className = "";
        if (message.includes("Expedited")) {
          className = "priority-expedited";
        } else if (message.includes("NoRush")) {
          className = "priority-norush";
        } else if (message.includes("Standard")) {
          className = "priority-standard";
        }
        
        // Add the notification
        notify(message, className);
        break;
      case "DeliveryScheduled":
        deliveryPopup.show();
        deliveryPopup.fadeOut(3000);
        
        // Also check for package priority info
        if (data.details.priority) {
          // Find the package with the name + "_package"
          const packageName = data.details.name + "_package";
          
          for (const [id, pkg] of packages.entries()) {
            if (pkg.name === packageName) {
              pkg.priority = data.details.priority;
              break;
            }
          }
          
          // Refresh UI if the priority management panel is open
          if (!changePriorityInput.hidden) {
            refreshPackagesLocally();
          }
        }
        break;
    }
  };

  loadScene(sceneFile);
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);

  renderer.setAnimationLoop(() => {
    let delta = clock.getDelta();
    time += delta;
    updateAnimations(delta);
    sendCommand("Update", { simSpeed: simSpeed });
    updateControls();
    renderer.render(scene, camera);
  });
});