import cv2
import numpy as np
import serial
import time

# Initialize serial communication with Arduino
try:
    arduino = serial.Serial('COM5', 9600)  # Replace with the correct port
    time.sleep(2)  # Allow time for Arduino to initialize
except Exception as e:
    print(f"Error: Unable to connect to Arduino: {e}")
    exit()

# Open the camera
cap = cv2.VideoCapture(0)

# Check if the camera is opened correctly
if not cap.isOpened():
    print("Error: Unable to open the camera.")
    exit()

# Define the ArUco dictionary and detector parameters
dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_250)  # Use your dictionary type
parameters = cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(dictionary, parameters)

# Prompt the user to input the marker ID
try:
    target_marker = int(input("Enter the ArUco marker ID (0 to 3) to stop the robot: "))
    if target_marker not in [0, 1, 2, 3]:
        raise ValueError("Invalid marker ID. Please enter a number from 0 to 3.")
except ValueError as ve:
    print(f"Error: {ve}")
    exit()

# Variables to track detection state
last_command_time = time.time()
cooldown_duration = 2  # Cooldown period in seconds
robot_stopped = False

try:
    while True:
        # Capture a frame from the camera
        ret, frame = cap.read()

        if not ret:
            print("Error: Unable to read the frame.")
            break

        # Detect markers in the frame
        markerCorners, markerIds, rejectedCandidates = detector.detectMarkers(frame)

        current_time = time.time()

        if markerIds is not None:
            # Extract detected marker IDs
            detected_ids = markerIds.flatten()
            print(f"Detected marker IDs: {detected_ids}")

            # Check if the target marker is detected
            if target_marker in detected_ids:
                print(f"Target marker {target_marker} detected!")

                # Send stop command if not already stopped
                if not robot_stopped and current_time - last_command_time > cooldown_duration:
                    print("Attempting to send STOP command to Arduino...") 
                    arduino.write(b'STOP\n')
                    print("Sent STOP command to Arduino.")
                    robot_stopped = True
                    last_command_time = current_time

            else:
                # Send continue command if the robot is stopped
                if robot_stopped and current_time - last_command_time > cooldown_duration:
                    arduino.write(b'CONTINUE\n')
                    print("Sent CONTINUE command to Arduino.")
                    robot_stopped = False
                    last_command_time = current_time

        else:
            print("No markers detected.")
            # Send continue command if no markers are detected and robot is stopped
            if robot_stopped and current_time - last_command_time > cooldown_duration:
                arduino.write(b'CONTINUE\n')
                print("Sent CONTINUE command to Arduino.")
                robot_stopped = False
                last_command_time = current_time

        # Display the frame with detected markers
        cv2.imshow("Frame", frame)

        # Exit if 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            print("Exiting...")
            break

except KeyboardInterrupt:
    print("Interrupted by user.")

finally:
    # Release resources and close connections
    print("Releasing resources...")
    cap.release()
    cv2.destroyAllWindows()
    arduino.close()
    print("Arduino connection closed.")
