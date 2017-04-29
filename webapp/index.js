// *****  Function for initial startup
ons.ready(function() {

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //Define the global variables
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////

    //photon-specific data
    var deviceID = "DEVICEIDGOESHERE";
    var accessToken = "ACCESSTOKENGOESHERE";

    //variables to use for sending/receiving with photon
    var setval = "setval";
    var getval = "getval";

    //Item prices
    var p1 = 9;
    var p2 = 12;
    var p3 = 15;
    var p4 = 6;
    var p5 = 3;

    //Item quantities
    var q1 = 0;
    var q2 = 0;
    var q3 = 0;
    var q4 = 0;
    var q5 = 0;

    //Item prep times
    var item1QT = 3;
    var item1PT = 1;
    var item2QT = 4;
    var item2PT = 7;
    var item3QT = 2;
    var item3PT = 5;
    var item4QT = 1;
    var item4PT = 3;
    var item5QT = 1;
    var item5PT = 1;

    //Item statuses (queue/prep/done)
    var status1 = -1;
    var status2 = -1;
    var status3 = -1;
    var status4 = -1;
    var status5 = -1;

    //Order-specific variables
    var orderPrice;
    var orderTime;
    var totalSubmit = null;

    //st. louis, mo, effective sales tax rate
    var taxRate = .07113;

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //Initial cosmetic tweaks
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////

    //Initial hides
    $("#order123").hide(); //hide the current orders
    $("#noOrders").show();

    //Lets disable all of the radios to start with
    $('input[name=soupRadio]').attr("disabled", true);
    $('input[name=pizzaRadio]').attr("disabled", true);
    $('input[name=burgerRadio]').attr("disabled", true);
    $('input[name=fryRadio]').attr("disabled", true);
    $('input[name=sodaRadio]').attr("disabled", true);

    // Set the initial values for the queue/prep times
    $("#item1QT").text(item1QT);
    $("#item1PT").text(item1PT);
    $("#item2QT").text(item2QT);
    $("#item2PT").text(item2PT);
    $("#item3QT").text(item3QT);
    $("#item3PT").text(item3PT);
    $("#item4QT").text(item4QT);
    $("#item4PT").text(item4PT);
    $("#item5QT").text(item5QT);
    $("#item5PT").text(item5PT);

    //Hide a debug/test item
    $("#testing1234").hide();

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //Initial send item ETAs to photon on startup
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////

    var initeta = "ieta#1q=" + item1QT + "#1p=" + item1PT + "#2q=" + item2QT + "#2p=" + item2PT + "#3q=" + item3QT + "#3p=" + item3PT + "#4q=" + item4QT + "#4p=" + item4PT + "#5q=" + item5QT + "#5p=" + item5PT;
    console.log("Submitting initial ETAS: " + initeta);
    sparkSend(initeta);
    console.log("initialETAStrings sent to photon");

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //Event listeners
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////

    //Submit button listener
    $("#submitOrderButton").click(function() {
        submitOrderClicked();
    });

    //Let's add listeners to the item status radios
    $('input[type=radio][name=soupRadio]').change(function() {
        changeItemStatus();
    });
    $('input[type=radio][name=pizzaRadio]').change(function() {
        changeItemStatus();
    });
    $('input[type=radio][name=burgerRadio]').change(function() {
        changeItemStatus();
    });
    $('input[type=radio][name=fryRadio]').change(function() {
        changeItemStatus();
    });
    $('input[type=radio][name=sodaRadio]').change(function() {
        changeItemStatus();
    });

    //Listener for the order complete button and reset button
    $('#completeOrderButton').click(function() {
        completeOrder();
    });
    $('#resetOrderButton').click(function() {
        resetOrders();
    });

    //Listeners for when item ETAs are changed
    $("#soupQUp").click(function() {
        changeTime();
    });
    $("#soupQDown").click(function() {
        changeTime();
    });

    $("#soupPUp").click(function() {
        changeTime();
    });

    $("#soupPDown").click(function() {
        changeTime();
    });

    $("#pizzaQUp").click(function() {
        changeTime();
    });

    $("#pizzaQDown").click(function() {
        changeTime();
    });

    $("#pizzaPUp").click(function() {
        changeTime();
    });

    $("#pizzaPDown").click(function() {
        changeTime();
    });

    $("#hamburgerQUp").click(function() {
        changeTime();
    });

    $("#hamburgerQDown").click(function() {
        changeTime();
    });

    $("#hamburgerPUp").click(function() {
        changeTime();
    });

    $("#hamburgerPDown").click(function() {
        changeTime();
    });

    $("#friesQUp").click(function() {
        changeTime();
    });

    $("#friesQDown").click(function() {
        changeTime();
    });

    $("#friesPUp").click(function() {
        changeTime();
    });

    $("#friesPDown").click(function() {
        changeTime();
    });

    $("#sodaQUp").click(function() {
        changeTime();
    });

    $("#sodaQDown").click(function() {
        changeTime();
    });

    $("#sodaPUp").click(function() {
        changeTime();
    });

    $("#sodaPDown").click(function() {
        changeTime();
    });

    //Used for debuggin purposes
    $("#valSubmit").click(function() {
        var number = $("#textValue").val();
        sparkSend(number);
        alert(number);
    });

    //Calculate the total each time an item quantity changes on order page
    $("#item1q").change(function() {
        calculateTotals();
    });
    $("#item2q").change(function() {
        calculateTotals();
    });
    $("#item3q").change(function() {
        calculateTotals();
    });
    $("#item4q").change(function() {
        calculateTotals();
    });
    $("#item5q").change(function() {
        calculateTotals();
    });

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function completOrder() runs when an order is marked complete
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function completeOrder() {

        //Let's reset all of the appropriate things back to no order values
        status1 = -1;
        status2 = -1;
        status3 = -1;
        status4 = -1;
        status5 = -1;
        $('input[name=soupRadio]').attr("disabled", true);
        $("input[name=soupRadio]").prop("checked", false);
        $('input[name=pizzaRadio]').attr("disabled", true);
        $("input[name=pizzaRadio]").prop("checked", false);
        $('input[name=burgerRadio]').attr("disabled", true);
        $("input[name=burgerRadio]").prop("checked", false);
        $('input[name=fryRadio]').attr("disabled", true);
        $("input[name=fryRadio]").prop("checked", false);
        $('input[name=sodaRadio]').attr("disabled", true);
        $("input[name=sodaRadio]").prop("checked", false);
        $("#order123").hide();
        $("#noOrders").show();
        $("#orderTimeSpan").text("");
        $("#orderTotalSpan").text("");
        $('.q1qty', '#order123')[0].innerHTML = "";
        $('.q2qty', '#order123')[0].innerHTML = "";
        $('.q3qty', '#order123')[0].innerHTML = "";
        $('.q4qty', '#order123')[0].innerHTML = "";
        $('.q5qty', '#order123')[0].innerHTML = "";
        $("#totalTotal").text("");
        $("#taxTotal").text("");
        $("#foodTotal").text("");
        q1 = 0;
        q2 = 0;
        q3 = 0;
        q4 = 0;
        q5 = 0;
        totalSubmit = null;

        //Now let's send the complete order command to the photon
        var completeOrderString = "completeorder#";
        console.log("Submitting: " + completeOrderString);
        sparkSend(completeOrderString);
        console.log("completeOrderString sent to photon");
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function resetOrders() runs when the restaurant hits the master reset button
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function resetOrders() {

        //Let's reset all of the appropriate things back to no order values
        status1 = -1;
        status2 = -1;
        status3 = -1;
        status4 = -1;
        status5 = -1;
        $('input[name=soupRadio]').attr("disabled", true);
        $("input[name=soupRadio]").prop("checked", false);
        $('input[name=pizzaRadio]').attr("disabled", true);
        $("input[name=pizzaRadio]").prop("checked", false);
        $('input[name=burgerRadio]').attr("disabled", true);
        $("input[name=burgerRadio]").prop("checked", false);
        $('input[name=fryRadio]').attr("disabled", true);
        $("input[name=fryRadio]").prop("checked", false);
        $('input[name=sodaRadio]').attr("disabled", true);
        $("input[name=sodaRadio]").prop("checked", false);
        $("#order123").hide();
        $("#noOrders").show();
        $("#orderTimeSpan").text("");
        $("#orderTotalSpan").text("");
        $('.q1qty', '#order123')[0].innerHTML = "";
        $('.q2qty', '#order123')[0].innerHTML = "";
        $('.q3qty', '#order123')[0].innerHTML = "";
        $('.q4qty', '#order123')[0].innerHTML = "";
        $('.q5qty', '#order123')[0].innerHTML = "";
        $("#totalTotal").text("");
        $("#taxTotal").text("");
        $("#foodTotal").text("");
        q1 = 0;
        q2 = 0;
        q3 = 0;
        q4 = 0;
        q5 = 0;
        totalSubmit = null;

        //Now let's send the reset order command to the photon
        var resetOrderString = "resetorders#";
        console.log("Submitting: " + resetOrderString);
        sparkSend(resetOrderString);
        console.log("resetOrderString sent to photon");
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function changeItemStatus() runs when the status of an order's item is changed
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function changeItemStatus() {

        //Get the item that was changed and it's new value
        var item = event.target.name;
        var value = event.target.value;

        //Start to piece together the string to send
        var sendString = "moditemstatus#";

        //Depending on the item, append the correct attribute, and send to photon
        if (item == "soupRadio") {
            status1 = value;
            console.log(item + " changed to " + status1);
            sendString = sendString + "i1=" + status1;
            sparkSend(sendString);
        } else if (item == "pizzaRadio") {
            status2 = value;
            console.log(item + " changed to " + status2);
            sendString = sendString + "i2=" + status2;
            sparkSend(sendString);
        } else if (item == "burgerRadio") {
            status3 = value;
            sendString = sendString + "i3=" + status3;
            console.log(item + " changed to " + status3);
            sparkSend(sendString);
        } else if (item == "fryRadio") {
            status4 = value;
            sendString = sendString + "i4=" + status4;
            console.log(item + " changed to " + status4);
            sparkSend(sendString);
        } else if (item == "sodaRadio") {
            status5 = value;
            sendString = sendString + "i5=" + status5;
            console.log(item + " changed to " + status5);
            sparkSend(sendString);
        } else {
            alert("error");
        }
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function changeTime() runs when queue or prep time of an item is changed
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function changeTime() {

        var p = event.target.id; // get which up/down arrow was pushed
        var changeTimeString = "modeta#"; //start to assemble the string

        //Determine which up/down arrow was pressed, append string accordingly 
        if (p == 'soupQUp') { //If soup queue time up was pressed
            item1QT = item1QT + 1; // add one to the time
            changeTimeString = changeTimeString + "1q=" + item1QT; //append the string
            $("#item1QT").text(item1QT); //set the new time in the html
        } else if (p == 'soupQDown') { //if soup queue down was presed
            if (item1QT > 1) { //if the time is >1 (don't subtract time if at 1, time can't be at 0)
                item1QT = item1QT - 1; //subtract 1
                changeTimeString = changeTimeString + "1q=" + item1QT; //append the string
                $("#item1QT").text(item1QT); //set the html text
            } else {
                alert("Can't go below 1");
            }
        } else if (p == 'soupPUp') {
            item1PT = item1PT + 1;
            changeTimeString = changeTimeString + "1p=" + item1PT;
            $("#item1PT").text(item1PT);
        } else if (p == 'soupPDown') {
            if (item1PT > 1) {
                item1PT = item1PT - 1;
                changeTimeString = changeTimeString + "1p=" + item1PT;
                $("#item1PT").text(item1PT);
            } else {
                alert("Can't go below 1");
            }
        }

        //pizza arrows
        else if (p == 'pizzaQUp') {
            item2QT = item2QT + 1;
            changeTimeString = changeTimeString + "2q=" + item2QT;
            $("#item2QT").text(item2QT);
        } else if (p == 'pizzaQDown') {
            if (item2QT > 1) {
                item2QT = item2QT - 1;
                changeTimeString = changeTimeString + "2q=" + item2QT;
                $("#item2QT").text(item2QT);
            } else {
                alert("Can't go below 1");
            }
        } else if (p == 'pizzaPUp') {
            item2PT = item2PT + 1;
            changeTimeString = changeTimeString + "2p=" + item2PT;
            $("#item2PT").text(item2PT);
        } else if (p == 'pizzaPDown') {
            if (item2PT > 1) {
                item2PT = item2PT - 1;
                changeTimeString = changeTimeString + "2p=" + item2PT;
                $("#item2PT").text(item2PT);
            } else {
                alert("Can't go below 1");
            }
        }

        //hamburger arrows
        else if (p == 'hamburgerQUp') {
            item3QT = item3QT + 1;
            changeTimeString = changeTimeString + "3q=" + item3QT;
            $("#item3QT").text(item3QT);
        } else if (p == 'hamburgerQDown') {
            if (item3QT > 1) {
                item3QT = item3QT - 1;
                changeTimeString = changeTimeString + "3q=" + item3QT;

                $("#item3QT").text(item3QT);
            } else {
                alert("Can't go below 1");
            }
        } else if (p == 'hamburgerPUp') {
            item3PT = item3PT + 1;
            changeTimeString = changeTimeString + "3p=" + item3PT;
            $("#item3PT").text(item3PT);
        } else if (p == 'hamburgerPDown') {
            if (item3PT > 1) {
                item3PT = item3PT - 1;
                changeTimeString = changeTimeString + "3p=" + item3PT;
                $("#item3PT").text(item3PT);
            } else {
                alert("Can't go below 1");
            }
        }

        //fries arrows
        else if (p == 'friesQUp') {
            item4QT = item4QT + 1;
            changeTimeString = changeTimeString + "4q=" + item4QT;
            $("#item4QT").text(item4QT);
        } else if (p == 'friesQDown') {
            if (item4QT > 1) {
                item4QT = item4QT - 1;
                changeTimeString = changeTimeString + "4q=" + item4QT;
                $("#item4QT").text(item4QT);
            } else {
                alert("Can't go below 1");
            }
        } else if (p == 'friesPUp') {
            item4PT = item4PT + 1;
            changeTimeString = changeTimeString + "4p=" + item4PT;
            $("#item4PT").text(item4PT);
        } else if (p == 'friesPDown') {
            if (item4PT > 1) {
                item4PT = item4PT - 1;
                changeTimeString = changeTimeString + "4p=" + item4PT;
                $("#item4PT").text(item4PT);
            } else {
                alert("Can't go below 1");
            }
        }

        //Soda arrows
        else if (p == 'sodaQUp') {
            item5QT = item5QT + 1;
            changeTimeString = changeTimeString + "5q=" + item5QT;
            $("#item5QT").text(item5QT);
        } else if (p == 'sodaQDown') {
            if (item5QT > 1) {
                item5QT = item5QT - 1;
                changeTimeString = changeTimeString + "5q=" + item5QT;
                $("#item5QT").text(item5QT);
            } else {
                alert("Can't go below 1");
            }
        } else if (p == 'sodaPUp') {
            item5PT = item5PT + 1;
            changeTimeString = changeTimeString + "5p=" + item5PT;
            $("#item5PT").text(item5PT);
        } else if (p == 'sodaPDown') {
            if (item5PT > 1) {
                item5PT = item5PT - 1;
                changeTimeString = changeTimeString + "5p=" + item5PT;
                $("#item5PT").text(item5PT);
            } else {
                alert("Can't go below 1");
            }
        } else {
            alert("SOmething went wrong");
        }

        //Log to console what we are submitting, and then send it to the photon
        console.log("Submitting CHANGE ITEM ETA: " + changeTimeString);
        sparkSend(changeTimeString);
        console.log("changeTimeString sent to photon");
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function calculateTotals() runs when an item quantity is inputted
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function calculateTotals() {
        console.log("calculate total called");

        //get the quantity of each item
        var q1 = document.getElementById('item1q').value;
        var q2 = document.getElementById('item2q').value;
        var q3 = document.getElementById('item3q').value;
        var q4 = document.getElementById('item4q').value;
        var q5 = document.getElementById('item5q').value;

        //calculate the food total
        var foodTotal = ((q1 * p1) + (q2 * p2) + (q3 * p3) + (q4 * p4) + (q5 * p5));
        var foodTotal2 = foodTotal.toFixed(2); //reduce to 2 places
        var foodTotalString = "$" + foodTotal2; //append $
        document.getElementById('foodTotal').value = foodTotalString; //set the html with the value

        //calculate tax total
        var taxTotal = (taxRate * foodTotal2); //calculate tax and set to 2 decimals
        var taxTotal2 = taxTotal.toFixed(2);
        var taxTotalString = "$" + taxTotal2;
        document.getElementById('taxTotal').value = taxTotalString;

        //calculate the overall total
        var totalTotal = parseFloat(foodTotal2) + parseFloat(taxTotal2); //caclculate overall total and change to.2 decimal places
        var totalTotal2 = totalTotal.toFixed(2);
        var totalTotalString = "$" + totalTotal2;
        document.getElementById('totalTotal').value = totalTotalString;

        totalSubmit = (parseFloat(totalTotal2)).toString(); //string of overall total

    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function submitOrderClicked() runs when submit button is clicked
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function submitOrderClicked() {

        //Send the item-specific etas in case they weren't already there (it happens!)
        var submitetas = "ieta#1q=" + item1QT + "#1p=" + item1PT + "#2q=" + item2QT + "#2p=" + item2PT + "#3q=" + item3QT + "#3p=" + item3PT + "#4q=" + item4QT + "#4p=" + item4PT + "#5q=" + item5QT + "#5p=" + item5PT;
        console.log("Submitting ETAS from submit order: " + submitetas);
        sparkSend(submitetas);
        console.log("submitetas sent to photon");

        //get the item quantities
        q1 = document.getElementById('item1q').value;
        q2 = document.getElementById('item2q').value;
        q3 = document.getElementById('item3q').value;
        q4 = document.getElementById('item4q').value;
        q5 = document.getElementById('item5q').value;

        //reset the item quantity htmls for cosmetic reasons
        $('#item1q').val("");
        $('#item2q').val("");
        $('#item3q').val("");
        $('#item4q').val("");
        $('#item5q').val("");

        //assume the order is invalid
        var validOrder = false;

        //if any item quantity was >1, the order is valid
        if (q1 > 0 || q2 > 0 || q3 > 0 || q4 > 0 || q5 > 0) {
            validOrder = true;

            //get the hour/min of the order
            var orderTimeTemp = new Date();
            var hours = orderTimeTemp.getHours();
            var minutes = orderTimeTemp.getMinutes();
            minutes = minutes < 10 ? '0' + minutes : minutes;
            hours = hours < 10 ? '0' + hours : hours;
            orderTime = hours + ':' + minutes;
            console.log(orderTime);
        }

        //just some logging
        if (validOrder == true) {
            console.log("VALID order based on q");
        } else {
            console.log("INVALID order based on q");
            return;
        }

        //go through each item, and if the quantity is 0/null/undefined, zero and disable that item on the order page
        if (q1 == null || q1 == undefined || q1 == 0) { //if item 1 is not in the order
            q1 = "0"; //set to 0
            $('input[name=soupRadio]').attr("disabled", true); //disable the radios
            $("input[name=soupRadio]").prop("checked", false); //uncheck the radios
            status1 = -1; //set the status to not in order
        } else { //else, we know item 1 is in the order
            status1 = 0; //set the status to in the queue
            $('input[name=soupRadio]').attr("disabled", false); // enable the radios
            $("input[name=soupRadio][value=0]").prop("checked", true); // check value=0 (queue) for this item
        }
        if (q2 == null || q2 == undefined || q2 == 0) {
            q2 = "0";
            $('input[name=pizzaRadio]').attr("disabled", true);
            $("input[name=pizzaRadio]").prop("checked", false);
            status2 = -1;
        } else {
            status2 = 0;
            $('input[name=pizzaRadio]').attr("disabled", false);
            $("input[name=pizzaRadio][value=0]").prop("checked", true);
        }
        if (q3 == null || q3 == undefined || q3 == 0) {
            q3 = "0";
            $('input[name=burgerRadio]').attr("disabled", true);
            $("input[name=burgerRadio]").prop("checked", false);
            status3 = -1;
        } else {
            status3 = 0;
            $('input[name=burgerRadio]').attr("disabled", false);
            $("input[name=burgerRadio][value=0]").prop("checked", true);
        }
        if (q4 == null || q4 == undefined || q4 == 0) {
            q4 = "0";
            $('input[name=fryRadio]').attr("disabled", true);
            $("input[name=fryRadio]").prop("checked", false);
            status4 = -1;
        } else {
            status4 = 0;
            $('input[name=fryRadio]').attr("disabled", false);
            $("input[name=fryRadio][value=0]").prop("checked", true);
        }
        if (q5 == null || q5 == undefined || q5 == 0) {
            q5 = "0";
            $('input[name=sodaRadio]').attr("disabled", true);
            $("input[name=sodaRadio]").prop("checked", false);
            status5 = -1;
        } else {
            status5 = 0;
            $('input[name=sodaRadio]').attr("disabled", false);
            $("input[name=sodaRadio][value=0]").prop("checked", true);
        }

        //cosmetic tweaks now that an order is submitted
        $("#order123").show();
        $("#noOrders").hide();
        $("#orderTimeSpan").text(orderTime);
        $("#orderTotalSpan").text("$" + totalSubmit);
        $('.q1qty', '#order123')[0].innerHTML = q1;
        $('.q2qty', '#order123')[0].innerHTML = q2;
        $('.q3qty', '#order123')[0].innerHTML = q3;
        $('.q4qty', '#order123')[0].innerHTML = q4;
        $('.q5qty', '#order123')[0].innerHTML = q5
        $("#totalTotal").text("");
        $("#taxTotal").text("");
        $("#foodTotal").text("");

        //Now let's get the correct string and send it to the photon with the quantities of each item and total price
        var orderSubmitString = "submitorder#q1=" + q1 + "#q2=" + q2 + "#q3=" + q3 + "#q4=" + q4 + "#q5=" + q5 + "#p=" + totalSubmit;
        console.log("Submitting: " + orderSubmitString);
        sparkSend(orderSubmitString);
        console.log("orderSubmitString sent to photon");
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function setValue() can be used to get data from the photon
    //It is currently inactive, but implemented on both ends (web + photon)
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function setValue(obj) {
        var newValue = document.getElementById('textValue').value;
        sparkSend(newValue);
    }

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    //function sparkSend() is the function that sends data over to the photon
    //it does so using post and a <64 character string (important!). The string
    //will be parsed on the photon side.
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    function sparkSend(newValue) {
        var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + setval + "/";
        $.post(requestURL, {
            params: newValue,
            access_token: accessToken
        });
    }

});