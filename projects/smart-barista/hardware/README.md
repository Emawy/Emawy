# Hardware and wiring

## Pin assignments from the uploaded sketch

| Function | Arduino digital pin |
|---|---|
| Base servo | 4 |
| Shoulder servo | 6 |
| Elbow servo | 8 |
| Wrist pitch servo | 9 |
| Wrist roll servo | 11 |
| Gripper servo | 12 |
| Ultrasonic trigger | 5 |
| Ultrasonic echo | 10 |
| Pump relay input | 7 |

Relay logic: HIGH = off, LOW = on. These are the code's numeric pin values; some original comments disagree. Confirm compatibility with the actual board.

## TO ADD

- Controller model and board revision.
- Exact servo models and quantities.
- Servo power-supply voltage/current ratings and grounding arrangement.
- Ultrasonic sensor, relay module, and pump specifications.
- Wiring diagram, clear electronics photos, and bill of materials.
- Calibrated joint limits and poses for the physical assembly.

The table is a source-code reference, not a verified wiring diagram. Power servos from a suitably rated supply and confirm voltage compatibility and common ground before testing.
