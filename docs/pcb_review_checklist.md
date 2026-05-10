# PCB Review Checklist

This is a board-level checklist for contest hardware and small embedded boards.

## Before placement

- define all connectors first
- lock the power input range
- decide the programming/debug port
- decide which pins are boot-critical
- mark test points for every rail you may need to probe
- decide where current flows in and out

## Schematic review

- check every power rail name and voltage
- verify decoupling near each IC power pin
- verify pull-ups and pull-downs on boot pins
- verify reset and enable pins
- verify crystal and clock pin requirements
- verify motor-driver direction and fault pins
- verify ADC reference and sensing range
- verify UART and I2C voltage levels

## Placement review

- keep the power loop short
- keep decoupling caps close
- keep crystals away from noisy edges
- keep high-current paths away from sensitive analog inputs
- keep the programming header reachable
- keep connectors easy to probe
- keep mounting holes clear of copper and parts

## Routing review

- route power and ground with obvious return paths
- avoid thin traces on high-current nets
- keep differential pairs together when needed
- avoid stubs on fast signals
- avoid accidental neck-downs near pads and vias
- keep analog and motor noise separated as much as possible

## Manufacturing review

- check silkscreen polarity marks
- check reference designators are readable
- check solder-mask openings
- check copper clearance around edges and holes
- check board outline and cutouts
- check drill sizes and annular ring
- check the BOM matches the footprint

## Bring-up review

- power the board with current limit first
- verify rails before inserting expensive chips
- verify programming access before routing anything else
- blink one LED first
- print one UART line first
- then test buttons, sensors, and motors one by one

## Contest shortcut

If time is short, review these first:

1. power
2. boot pins
3. programming header
4. decoupling
5. current paths
6. test points
7. connector polarity
8. one clean bring-up firmware
