# Bot X13217 source code 
---
Code is written by Shiroko as the player in the match (will)

Thus, It is not only my work, thanks to all of my teammates to build the bot.

Also thanks to the developers of all the fantastic tool-chains.

And a special thanks to Mr.Wang, who gives a huge help to all of us.

---

### The code is written with **Emacs**, the best editor I ever met.
### The code is reviewed and debuged with **Visual Studio Pro**, the most convenient "code viewer" I ever met.
> #### Actually, emacs with evil inside is the best  (kira✨)

---

## * Design
    

## * Abstract and code reuse

Header & models:

    - Arm model (arm.h)
    - Bullet Fire model (bullect_fire.h)
    - Collector model (collector.h)
    - Movement model (move.h)
    - Joystick model (joycon.h)

Classes:

    * Arm class
        - Constructor:
            params:
                * a pointer SmaerServo instance which is initialized by setup to control hands.
                * an int[2] array which defines the index of hands.
                * an int value "lift" which defines the index of lift DC motor (m9 as 1 - default).
                * an int value "flip" which defines the index of flip DC motor (m10 as 2 - default).
            This function processes the initialization and reset the smart servo. 
        - SwitchHand:
            params:
                * an int value defines which hand will be manipulated (0 means all of two).
            This function will reverse the hand(s).
        - SwitchFlip:
            Flip the arm! Let's rock and roll~
        - SwitchLift:
            params:
                * an int value defines the power for lift motor (maxium is 255).
            Lift it! Get the score!
        - loop:
            params:
                * a **long** value defines the current time of caller.
            Check the motor status and triggered time to decide wheather stop the motor. (Others are the same)
        
    * Bullet Fire class
        - Constructor:
            params:
                * a pointer SmaerServo instance which is initialized by setup to control the angle to fire.
            This function processes the initialization and reset the smart servo. 
        - SwitchFire:
            Fire! Fire! Fire!
        - SwitchAngle:
            Shoot'em out! All of them!
        - Power:
            Well... Get it to maxium power!
        - loop:
            Like `loop` in class arm.

    * Collector class
        - Constructor:
            params:
                * an int value defines the index of motor
            Init.... Like the others.
        - Switch:
            ... Swipe the bullet in.
        
    * Joycon class
        - Constructor:
            params:
                * a pointer of MePS2 wireless controller instance.
                * a pointer of Print instance for output the log (not yet support, you can pass `Serial` to it).
            Init...
        - begin:
            params:
                * a long value of byterate, default it 115200. uesd for ps2 initialization.
            Initialize ps2
        - append_invoke_func:
            params:
                * a function pointer to handler which is received Joycon_Status for handle.
            Append a handler to invoke list
        - remove_invoke_func:
            params:
                * a int value of function's id in invoke list which will be removed
                * a function pointer to handler which is to be removed
            Appending needs removing!
        - loop:
            Loop like a circle, get data, copy and distribute to invoke list.
        
    * Move class
        - Constructor:
            params:
                * a int[4][2] array of wheel motor (X1, X2, Y1, Y2). 1st of cuple is driver id, other is motor id.
            Init
        - begin:
            Init motor
        - handle_joycon_x and handle_joycon_y:
            params:
                * Joycon_Status structure, contains data recived from joystick.
            Just move it~
        
## * Problem & Solving

## * Automatical Control Part