# PCB AI and Layout Tooling

This page collects the easiest official-looking tools for board work in EasyEDA Pro / JLCEDA.

The goal is not to replace the engineer.

The goal is to remove the repetitive parts:

- pin mapping
- outline and mounting-hole generation
- symbol/pin sanity checks
- DRC-style review
- fast routing assistance
- bridge-based AI control

## What the Bilibili video is pointing at

The video `BV1V19DBAEcE` is titled `告别熬夜布线 AI 干翻428小时PCB活`.

That strongly suggests an AI-assisted PCB-layout workflow, not a magical one-click finished board.

Practical meaning:

- AI can help with repetitive board work
- AI can help with rule-based generation and review
- AI still needs your schematic, constraints, and sign-off

## Official bridge path

The official JLC docs list these companion projects:

- `eext-run-api-gateway`: `EDA <-> AI 网关扩展`
- `easyeda-api-skill`: `扩展 API AI Skill`

Together, they form the useful path for programmatic EasyEDA control.

Basic architecture:

1. EasyEDA Pro loads the `run-api-gateway` extension.
2. The bridge server listens on ports `49620-49629`.
3. An AI agent or tool connects through the bridge.
4. The agent reads or edits the current EDA window.

## What I would install first

### Must-have

- `Run API Gateway` v1.0.5
- `EasyEDA API Skill` v1.1.3
- `PCB自动化工具` v2.4.1
- `板框定位孔生成助手` v1.2.1
- `图片轮廓提取器` v1.4.1
- `Pacwoman-封装DRC检查` v26.3.30

### Very useful

- `PCB RBR Autorouter` v1.2.3
- `AI 设计助手` v2.6.1
- `MCP Bridge` v1.5.4
- `JLCEDA AI Agent` v0.5.3
- `Design Copilot` v1.0.5
- `CubeMX 引脚配置导入原理图` v1.0.0

### Nice extras

- `PCB 电流密度仿真` v2.0.0
- `WaveDrom时序图` v1.0.2
- `交互式BOM` / `交互式BOM(移植版)`
- `PowerBoys-芯片外围配置方案管理器`
- `内电层自动分划`

## What each one is good for

- `Run API Gateway`: bridge EasyEDA Pro to AI and external tooling
- `EasyEDA API Skill`: API reference plus bridge-based automation
- `PCB自动化工具`: fanout, routing helpers, placement helpers, and other repetitive PCB actions
- `板框定位孔生成助手`: fast board outline plus mounting-hole setup
- `图片轮廓提取器`: turn reference images into board outlines or contours
- `Pacwoman-封装DRC检查`: package and footprint review
- `PCB RBR Autorouter`: routing assistance when you want a first-pass route
- `AI 设计助手` / `Design Copilot` / `JLCEDA AI Agent`: AI-guided board work and review
- `CubeMX 引脚配置导入原理图`: STM32 pin mapping from firmware config into schematic work
- `PCB 电流密度仿真`: sanity check for current handling
- `WaveDrom时序图`: help turn timing notes into readable documentation

## Good contest-board stack

If the board is for an embedded contest, I would start with this stack:

1. `Run API Gateway`
2. `EasyEDA API Skill`
3. `PCB自动化工具`
4. `板框定位孔生成助手`
5. `图片轮廓提取器`
6. `Pacwoman-封装DRC检查`
7. `CubeMX 引脚配置导入原理图`

That gives you a practical flow for:

- MCU pin mapping
- board outline creation
- footprint review
- manual routing with assistance
- AI-assisted follow-up edits

## What still needs human review

- power-tree correctness
- decoupling placement
- return-path integrity
- connector polarity
- boot-strap pins
- current limits
- thermal and EMI risk
- final DRC and manufacturer rules

## Useful sources

- [Bilibili video](https://www.bilibili.com/video/BV1V19DBAEcE)
- [JLC extension marketplace](https://ext.lceda.cn/?jlcGroup_source=jlc)
- [JLC ancillary projects docs](https://prodocs.lceda.cn/cn/api/guide/ancillary-projects.html)
- [EasyEDA API Skill](https://clawhub.ai/skills/easyeda-api)
