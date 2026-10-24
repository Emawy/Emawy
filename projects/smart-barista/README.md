# Smart Barista Robotic Arm

A servo-driven robotic arm project combining programmed cup handling, ultrasonic cup detection, and relay-controlled dispensing.

The supplied Arduino sketch coordinates six servos through a predefined pick-and-place sequence. This portfolio includes the original firmware, SolidWorks assembly and parts, and a supplied Windows application.

## Demo video

**TO ADD — project demonstration video.**

Add your video or a YouTube link using the instructions in [media/README.md](media/README.md), then replace this line with the working video link.

<!-- VIDEO_LINK_HERE: Replace with [Watch the project demo](YOUR_VIDEO_URL). -->

**Project photo:** TO ADD in `media/images/`.

## Project files

| Folder | Contents |
|---|---|
| [firmware/](firmware/) | Original Arduino sketch, unchanged |
| [cad/solidworks/](cad/solidworks/) | 1 SolidWorks assembly and 16 part files, with original filenames |
| [hardware/](hardware/) | Pin assignments from the sketch; wiring and component details to add |
| [software/windows/](software/windows/) | Supplied Windows executable; source code unavailable |
| [media/](media/) | Marked locations for the demo video and project photos |

## Programmed sequence

1. Move from home to the approach position.
2. Lower the arm and close the gripper.
3. Lift and transport the cup while holding the gripper closed.
4. Release the cup and return through a clearance pose to home.
5. Read the ultrasonic sensor and activate the pump relay when measured distance is below 10 cm.
6. Pause before repeating the cycle.

Movement uses cubic easing between fixed joint angles, with a nominal six-second duration per movement. These are programmed joint poses; inverse kinematics and AI perception are not implemented in the supplied sketch.

## Hardware and software

The code uses the Arduino `Servo.h` library, six servo channels, ultrasonic trigger/echo signals, and an active-low pump relay. CAD filenames include MG996R and Micro 9g servo models; exact installed quantities and controller model still need confirmation.

See [hardware/README.md](hardware/README.md) for the pin table and missing setup details.

## Open the files

- **Firmware:** Open [sketch_may22a.ino](firmware/sketch_may22a/sketch_may22a.ino) in Arduino IDE. Its parent folder matches the sketch name. Select the actual board and port and check the Servo library before compiling. Review the issues below before hardware operation.
- **CAD:** Download the repository, keep all files in `cad/solidworks/` together, and open `CANHTAY_ROBOT.SLDASM` in a compatible SolidWorks version. References and geometry have not been validated in SolidWorks.
- **Windows application:** Archived as supplied. It was not executed or validated; its dependencies and relationship to this firmware are unconfirmed.

## Current status and known limitations

Original uploaded files are preserved byte for byte. Documentation is based on source inspection, not a hardware test or successful compilation.

- `checkCupPresent()` is declared `bool` but does not return a value; its caller uses that undefined return value in an `if`.
- `pulseIn()` has no explicit timeout, and a zero reading is treated as a distance below 10 cm, which can trigger dispensing when no valid echo was received.
- Actual pump-on delay is **4 seconds**, although nearby comments say 2 seconds.
- Several pin comments and gripper-angle comments disagree with the executable code; the pin table follows the declared values.
- Motion and pump operation use blocking loops/delays; no emergency-stop input, homing feedback, or collision detection is implemented.
- Joint limits and stored poses need physical calibration. A servo's `read()` reflects its commanded angle, not measured joint position.

## Complete the portfolio

- [ ] Add the demo video link above.
- [ ] Add a project overview photo.
- [ ] Confirm the controller, servo quantities, supply ratings, sensor, relay, and pump.
- [ ] Add the wiring diagram and bill of materials.
- [ ] Explain individual contributions and credit collaborators.
- [ ] Confirm the original CAD source, designer, and redistribution license; supplied CAD is not claimed here as an original mechanical design.
- [ ] Add the Processing/application source if available.
- [ ] Correct and test the firmware issues in a separate revision.
- [ ] Add measured results, such as repeatability and dispensing volume.

No project-wide license is assigned until third-party asset permissions are confirmed.

[Back to profile](../../README.md)
