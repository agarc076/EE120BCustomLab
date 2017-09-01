# EE120BCustomLab


In this lab, I created a version of the Simon Game. The Game's starting state waits for the user to press any button in order to start. After the user pressed any button, the XOR Shift function would generate 9 sequences into an array, iterating the number of sequences onto on board LED's. Then the user would have to gradually have to remember and enter the correct sequential pattern that corresponded to the displayed LED's by pressing the corresponding buttons. if user is able to completely iterate through 9 levels of random sequential patterns then the user wins, but if at any momment the user is not able to enter the generated sequence then the user loses.

In this game the only rule is that the user needs to follow the onscreen instructions. When the system is ready to recieve the user's input data, a message on the LCD screen will appear saying "waiting for input: ".

To succesfully implement the Simon game I had to utilize the IEEE kit that included an atmega 1284, an LCD screen, a 5V power supply, 4 buttons and an LED bar. To program the atmel chip I used the atmel studio included with the purchase of the processor. In order to randomize the LED sequences I used an XOR32 shift from wikipedia. http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/ https://en.wikipedia.org/wiki/Xorshift


