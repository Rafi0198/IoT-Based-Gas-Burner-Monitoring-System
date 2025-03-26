# IoT-Based-Gas-Burner-Monitoring-System
The designed system detects any gas leakage/fire from the burner and alerts the authorized person via IoT dashboard/Email/SMS/Phone Call having terminated the burner immediately. The child ESP32 board has been used to detect the presence of human inside the kitchen more precisely.

Features: 

1. If the Gas Burner is kept on in absence of human: 
(i) A Red Bulb will start flashing.
(ii) A Buzzer will start buzzing synchronous to the Red Bulb.
(iii) An Email will be sent to the respective person. 
(iv) A Phone Call & an SMS will be sent to the respective person. 
(v) The alert will be updated in the Website & Android Application. 

2. If Gas leaks from the Burner in absence of human: 
(i) A Yellow Bulb will start flashing.
(ii) A Buzzer will start buzzing synchronous to the Yellow Bulb.
(iii) An Email will be sent to the respective person. 
(iv) A Phone Call & an SMS will be sent to the respective person. 
(v) The alert will be updated in the Website & Android Application. 
Additional: Burner will automatically turn off in case 1 & 2. 

3. If the Gas Burner is kept on in presence of human: 
(i) A Red Bulb will turn ON.

4. If Gas leaks from the Burner in presence of human: 
(i) A Yellow Bulb will turn ON.
Additional: The temperature & the humidity will be monitored as well.

Monitoring & Alerting Hierarchy: 
LCD Display → AC Colour Bulbs → Buzzer → Call → SMS → Email → Website → Android Application 

Note: The child ESP32 board has been used to detect the presence of human inside the kitchen more precisely. Again the free version of the cloud service allows maximum of 2 Things & 5 Variables at most under each thing. So, to accommodate all the monitoring parameters, we need the child ESP32 board so that we can couple 2 things with 2 devices under the same monitoring dashboard.  

