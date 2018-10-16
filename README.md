# opendlvPs3Controller

Use PS3 Controller to control to accelerate, brake, and steer.

## Buttons to actions

![alt text](http://wiki.ros.org/ps3joy?action=AttachFile&do=get&target=ps3_buttons.jpg "Logo Title Text 1")

![alt text](http://wiki.ros.org/ps3joy?action=AttachFile&do=get&target=ps3_buttons_front.jpg "Logo Title Text 1")

![alt text](http://wiki.ros.org/ps3joy?action=AttachFile&do=get&target=ps3_axes.jpg "Logo Title Text 1")

RIGHT ANALOG STICK Y #3 => Ground Speed Request (percentage)

LEFT ANALOG STICK X  #0 => Ground Steering Request (percentage)

RIGHT ANALOG STICK X #2 => PWM Request (BrakePwm,-32768 to +32767)

BUTTON SELECT           => Switch State Reading(set go signal msg)

RIGHT ANALOG BUTTON #2  => Switch State Reading(set finish signal msg)
