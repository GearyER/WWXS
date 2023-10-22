# WWXS
This is an updated and merged repository to house the content that was originally kept across six different repos. For purpose of organization and proper version tracking, all further additions to the project should be kept here. 

## Introduction
The WWXS project consists of three parts: A buoy, an app, and a website.
The buoy has a collection of sensors attached to it which all collect data from a body of water over the course of 30 days. The sensors are for salinity, turbidity, temperature, insolation, and soon dissolved oxygen. The code for the buoy, written in C, is run from an ESP32 Sparkfun.
The app connects to the buoy via Bluetooth and allows a user to log in to a buoy they own and begin collecting data from the water or downloading the data that has already been gathered. It displays the averages for the data on the user’s phone, then uploads the data to the website.
The website displays all the collected data from the various buoys. The data can be used for interpreting the overall health of the water.

## Documentation
### Deliverables
Files such as SDS (Software Design Specification) and Vision and Scope documents are PDF files submitted to Western Washington University. These should be uploaded once and not edited as they are atomic milestones to the project. Further changes should uploaded as a new file.

### Upload Instructions
1. Title document in the format yearQuarter.pdf where the year and quarter are replaced with the year and quarter the document was submitted (i.e. 2023Spring.pdf)
2. Place the document in the corresponding folder (ex. Documentation/SDS)
3. If the documentation does not fit into any of the existing categories, name your document as standard (ex. BuoySetupDoc.pdf) and place into the Documentation folder

### Old Documents
Files that are more than 2 years old have been put in a separate subfolder in the Documentation folder in order to avoid clutter. These documents are typically out of date but still serve as useful reference.

## Buoy
Contains the codebase for the SparkFun ESP32 Thing.

### Development Environment
The guide for setting up the environment needed to compile the code and flash it to the ESP32 board is found [here](https://docs.espressif.com/projects/esp-idf/en/v4.2.1/esp32/get-started/index.html).
Dedicated installation instructions found [here](Buoy/INSTALL.md).

### Building the Buoy
In-depth instructions, information, and guides involving the creation of the buoy and its integral parts can be found in the section titled "Buoy" in this document:
WORK IN PROGRESS, UPDATE WHEN COMPLETE


## App
WORK IN PROGRESS
* What is contained in the folder
* Features
* Prerequisites, installation instructions

## Website
WORK IN PROGRESS

### Documentation
Servers as our website documentation - contains docker information for pushing our website. For further documentation, please check out our [Website/documentation](https://github.com/GearyER/WWXS/tree/main/Website/documentation).

### Web-API
Servers as our backend - updating, creating, and accessing buoy data. For further documentation about features, developing environment, and more, please check out our [web-api](https://github.com/GearyER/WWXS/tree/main/Website/web-api).

### Web-UI
Servers as our user interface - users can log-in and view buoy data. For further documentation about features, developing environment, and more, please check out our [web-ui](https://github.com/GearyER/WWXS/tree/main/Website/web-ui).

## Contribution
This was previously a revolving door project. New developments have given this project a set endpoint. Future contributions after June 2024 are allowed at the discretion of
Western Washington University's Computer Science department, the SEA Discovery Center in Poulsbo, and the Department of Defense.

For current contributors, please create a new branch when working on any updates to the code to avoid merge conflicts or breaking anything.\
\> git branch [branch name] //create new branch.\
\> git checkout [branch name] //change your working branch to the new one you created.\
\> git branch //see current existing branches and which one you are currently working from.\
**Please only create new branches from the main branch**

## License
As of Spring 2023, this project is now under the **MIT License**. See the [license](LICENSE) document for details.
