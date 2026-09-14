# Firmware workspace

No executable robot firmware is provided yet. See [the development plan](../docs/PLAN.md).

Implement diagnostics before motion: serial/I2C scan, OLED, IMU, PCA9685 single-servo calibration, then integrated behavior. Use XIAO ESP32C3, I2C GPIO6/GPIO7, and PCA9685 channels 0-3. Keep motion disabled until commanded and apply calibrated limits. Pin dependency versions after a successful compile; record hardware tests separately from compile results.
