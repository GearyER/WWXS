Feature: Receiving Data

    Pulling data from database
    Scenario: Pulling data from database when there is data
    Given User uploaded data at least 1 time to the database 
    When User request their data from the website: 1
    Then The website should send the data to the user

    Scenario: User does not want to pull data from the database
    Given User uploaded data at least 1 time to the database 
    When User request their data from the website: 0
    Then The website should send the data to the user

    Scenario: User want to pull data from the database but the user did not upload any file yet
    Given User uploaded data at least 0 time to the database 
    When User request their data from the website: 1
    Then The website should send the data to the user