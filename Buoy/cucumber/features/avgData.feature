Feature: Average Data

    Getting Average Data
    Scenario: average data for mulitple days
    Given The temperature of the lake was 36 degrees on day 1
    And The temperature of the lake was 40 degrees on day 2
    Then The average temperature of the lake would be 38 degrees

    Scenario: average data for mulitple days
    Given The temperature of the lake was -4 degrees on day 1
    And The temperature of the lake was 12 degrees on day 2
    And The temperature of the lake was -2 degrees on day 3
    Then The average temperature of the lake would be 2 degrees

    Scenario: average data for mulitple days 
    Given The temperature of the lake was 40.3 degrees on day 1
    And The temperature of the lake was 30.2 degrees on day 2
    And The temperature of the lake was 4.20 degrees on day 3
    Then The average temperature of the lake would be 24.9 degrees
