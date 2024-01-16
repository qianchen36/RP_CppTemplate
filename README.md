# RobotPilots Cpp Template

Object-oriented develop library for RoboMaster EC.

### About
This template is designed from System-Level, which means you need a Board-Level support library to initialize hardwares and provide HAL APIs.

There is a BSP examples in `Boards/Dbldeck_MasterCtrl` folder, using STM32CubeMX for fast develop.

### Project Paths
- `Applications/` Library files

- `Boards/` Board-Level support library (BSP)

- `Drivers/` Hardware abstract library (HAL)

- `Middlewares/` Middlewares' library (FreeRTOS, CMSIS-DSP, ...)

### Application Paths
- `Applications/Config/` Application configurations

- `Applications/Algo/` Build-in algorithms

- `Applications/Comm/` Communicate abstract

- `Applications/Device/` Device abstract

- `Applications/Module/` Module abstract

- `Applications/System/` System abstract
