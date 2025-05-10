Team number: 001-24
Team members: Surafiel Ghirmai,  Helina Kidane

Overview:
This project is a package delivery simulation where users can schedule and watch deliveries carried out by drones. The system allows users to create different types of entities—like drones, robots (receivers of the package), and the packages themselves. Once scheduled, drones automatically pick up the packages and deliver them to the correct robot using various movement strategies. The whole process is visualized in a 3D simulation that shows the pickup, travel, and drop-off in real time. For our extension, we added a priority-based delivery system. Now, when scheduling a package, users can choose a priority level: Expedited, Standard, or No Rush. This affects how the delivery queue is managed, higher priority packages get delivered first, while lower priority ones wait their turn. Behind the scenes, this feature is implemented using the State design pattern, where each priority level is represented as its own state class. This makes it easy to manage and switch between priorities while keeping the code clean and flexible. This system gives users better control over how deliveries are handled. We also introduced a centralized Air Traffic Control system to manage flying entities like drones and helicopters. It keeps track of all airborne entities and makes sure there’s at least a 50-unit safety buffer between them. This was implemented using the singleton pattern to ensure global coordination, along with the decorator pattern to automatically register and monitor every flying entity. If a potential collision is detected, the system reroutes or deflects the entity to keep the skies safe.

Instructions:
Create a Delivery:
  Click the "Schedule Trip" button and select pickup and drop-off locations. You must enter a name and choose a shipping priority (Expedited, Standard, or No Rush) before submitting.
View the Priority Queue:
  Click "Refresh Packages" to see all undelivered packages listed by priority. Expedited ones are listed first, No Rush packages are last.
Change Priority:
  You can change a package's priority before it gets picked up. Click "Change Priority," enter the package name, and select the new priority. Then click update.

Requirements:
The system shall allow users to assign a shipping priority (Expedited, Standard, or No Rush) when creating a package.
When a user schedules a delivery, the system shall store the assigned priority and place the package in a priority-based queue
While a package is in the queue and has not been picked up, the system shall allow its priority to be changed.
The system shall prevent priority changes once the package has been picked up or delivered.
The system shall always select the highest-priority undelivered package for delivery.
While two or more packages share the same priority level, the system shall deliver them in the order they were added.
The system may notify the user when a package’s priority is successfully changed.
The front-end UI shall display the delivery queue, showing packages grouped and ordered by priority.
When a package is delivered, the system shall remove it from the queue.


Design:
Our extension builds on the existing delivery system by introducing a priority-based shipping feature using the State design pattern. Each package is assigned a PriorityShippingState(Expedited, Standard, or No Rush) which determines its urgency in the delivery queue. This pattern allows each priority level to encapsulate its own behavior (such as its name and priority value) and enables clean, modular handling of priority logic without cluttering the main package class. When a package is created or its priority is changed, it transitions to the appropriate state, which affects how the queue is sorted and which package is selected next for delivery. This approach makes the system flexible, easy to extend with new priority types, and ensures a clean separation of concerns.

Sprint retro:
One of the things that went well in this sprint was our overall implementation of the priority-based shipping system. I was able to successfully use the State pattern to manage package priorities, and the backend logic worked as expected, packages were properly sorted and selected for delivery.  I also made good use of the existing system by integrating cleanly with the entity creation, scheduling, and update flows.
What didn’t go well was the frontend integration, particularly getting the No Rush priority to show up in the delivery queue display. While the backend correctly registered and handled No Rush packages, they never appeared visually in the list like the other priority levels. This led to a lot of frustration and time spent debugging the issue. I tried researching how the frontend renders data from the queue, and experimenting with how the data was passed. Next time, I would plan to allocate more time for frontend testing and research earlier in the sprint. We also would have prioritized integrating frontend feedback sooner, so we could identify visualization bugs earlier and coordinate more effectively between the backend and frontend logic


