# Andrew's RoboDog Lab

Local TypeScript + Three.js practice app. Original procedural 3D dog with tilt sliders, four animated expressions, center reset and pause/resume. Practice data only: no serial access, hardware writes, sensor readings or physical-motion claims. This is a character, not a validated mechanical simulation.

From this directory with Node.js 22.12+ (or a compatible newer release):

```powershell
npm ci
npm run dev
```

Open the localhost URL printed by Vite in a browser; do not open index.html directly or the TypeScript source as a document. `npm run build` type-checks and creates the static app in ignored dist/. All assets are local; no external fonts or models are downloaded at runtime.

Try the lean and pitch sliders, reset to center, select each expression, and pause/resume idle animation. Pause freezes idle motion; deliberate slider changes still adjust pose. Reduced-motion preference starts idle animation paused. WebGL2 is required and failure displays a message.

Live sensor connection is a later milestone. The seven firmware lessons and book are unchanged.

## Sensor playground

All seven sliders are simulated, not connected to the ESP32. Drag the scene to orbit; scroll to zoom.

1. Predict: what happens when sideways acceleration increases? Open Accelerometer and move X. Scout leans and shifts. Y above 9.81 produces a bounce; Z produces a forward reaction. These bounded movements are authored animations, not position tracking.
2. Open Gyroscope. Set Y to 30 degrees/second. Scout keeps turning. Return it to zero to stop adding rotation. Try X and Z individually, then together. Reset restores the starting pose.
3. Open Temperature. Below 25 Celsius Scout shivers; above 38 he pants with a tongue. These fictional comfort bands refer to chip temperature, not ambient temperature, animal health, or hardware safety limits.
4. Mix the inputs. Pause freezes body motion; Reset clears all inputs and orientation. The camera remains usable while paused.

Practice coordinates are X right, Y up, Z toward the nose. Gyroscope rates integrate with quaternions. Acceleration reactions deliberately do not implement a physical trajectory or sensor-fusion estimator. The floor clearance correction is visual staging, not collision physics. Real MPU6050 integration still requires board-axis mapping, stationary bias calibration, gravity separation, timestamps, stale-data handling, and orientation estimation. There is no magnetometer to prevent absolute heading drift.

Validation: TypeScript and production bundle build passed. Browser inspection confirmed the rendered dog, three-axis controls, changing values and cold-temperature reaction label. Physical sensors and motors were not accessed. Firmware and book remain unchanged.
