# Things-locating
Using C++ and OpenCV library to establish functions could locating things on purely color background and adjust the perspective and lens distortion by four points. Then generating Arduino code to control Braccio arm automatically.  
## Demand
- The color of background under the things you want to recognized have to be pure.  
- In order to ensure the shadow will not affect the result, I strongly recommend that the background color shall be black.
- There are four points located in the four corners which have to be different color from background.
- The things shall not to be outside the four points.
