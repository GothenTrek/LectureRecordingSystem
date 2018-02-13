# LectureRecordingSystem

A Lecture Recording System to aid in revision and catchup of university modules. Utilising OpenCV to identify opportune times to focus a whiteboard


Lecture Recording System User Guide
Introduction
The Lecture Recording System is a program designed to record handwritten text within a lecture. This can benefit both students and lecturers when it comes to exam revision and assignments. The program is able to run on most windows machines. 
Requirements
The program requires a computer system with at least a dual core CPU, running a Windows OS. A external camera is required for this software to operate.
Setup
If running an older version of windows the drivers for the external camera will need to be installed manually. These can usually be found on the manufacturers website. The webcam will need to be connected to an operational computer. The computer will need to be in the necessary location for lecture recording.
Double click the executable Project76.exe, this will open the program and should display the view from the external camera. 
The program is now ready to take input. Before lecture recording can begin the threshold will need to be set. To aid with this the zoom function can be manually enabled by pressing ‘u’ on the keyboard, this will zoom in. Once zoomed to an appropriate level pressing ‘CTRL’ will toggle the text enhancer. From this point tapping ‘[‘ will reduce the threshold and tapping ‘]’ will increase the threshold. Once the threshold is set. Press ‘r’ to reset the zoom to default. Press ‘CTRL’ to toggle the text enhancer off. 
Starting Lecture Recording
To begin, ensure that the top of the whiteboard is parallel to the camera frame. Ensure that the white board is cleared. Ensure that there are no obstacles in front of the white board. The cameras tilt may need to be adjusted. To start the lecture recording hold the ‘Shift’ key and click on the two diagonally opposing corners of the whiteboard. The program output should halt temporarily, once the program has continued displaying output from the camera the lecture recording has now started.
Ending Lecture Recording
To end the lecture recording, press the escape key, noted ‘Esc’ on the keyboard. This will close the program.


Correcting Board Location
If the board location has been incorrectly defined, hold ‘Shift’ key and click on the two diagonally opposing corners again. This will correct the boards location.
Text Enhancements
This program has additional optional text enhancements: Sharpening & Edge Detection. To enable edge detection, press ‘e’. To enable Sharpening press ‘s’. 
Upscaling
The upscaling mode can be changed between: Linear, Lanczos and Nearest. To change the mode press ‘m’. *Lanczos can severely impact performance.*
Complete Control List
Select White Board	SHIFT + LEFT CLICK * 2
Manual Zoom In	U
Manual Zoom Out	D
Reset Zoom	R
Toggle Text Enhancer	CTRL
Reduce Threshold	[
Increase Threshold 	]
Toggle Sharpening	S
Toggle Edge Detect	E
Change Upscaling Method	M
Quit Program/End Recording	ESC

