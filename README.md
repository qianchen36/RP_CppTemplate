# RobotPilots Cpp Template

Object-oriented develop library for RoboMaster EC.

### About
This template is designed from System-Level, which means you need a Board-Level support library to provide BSP_APIs needed by this template.

There is a BSP examples in `Boards/GimbalController` folder, using STM32CubeMX for fast develop.

### Project View
- `Applications/` Library files

- `Boards/` Board-Level support library (BSP)

- `Drivers/` Hardware abstract library (HAL)

- `Middlewares/` Middlewares' library (FreeRTOS, CMSIS-DSP, ...)

### Library View
- `Applications/Config/` Library configurations

- `Applications/Algo/` Build-in algorithms

- `Applications/Comm/` Communication support

- `Applications/Device/` Device abstarct

- `Applications/Module/` Module abstarct

- `Applications/UserTask/` User defined tasks for FreeRTOS
