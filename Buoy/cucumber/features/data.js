const { Given, When, Then } = require('@cucumber/cucumber')
let x = 0
let y = 0
let count = 0

Given('User uploaded data at least {int} time to the database', function (int) {
    x = int
});

When('User request their data from the website: {float}', function (float) {
    y = float
});

Then('The website should send the data to the user', function () {
    if(x > 0 && y == 1)
    {
        console.log("PASS: Show data")
    }
    else
    {
        console.log("FAIL")
    }
    x = 0
    y = 0
});

Given('The temperature of the lake was {float} degrees on day {int}', function (float, int) {
    x = x + float
    count = int
});

Then('The average temperature of the lake would be {float} degrees', function (float) {
    let check = x/count
    if(float == check.toFixed(1)) {
        console.log("PASS", check.toFixed(1))
    }
    else
    {
        console.log("FAIL")
    }
    x = 0
});