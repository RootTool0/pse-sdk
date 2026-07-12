<img src="../../../assets/portal_solver_logo.png" width="60m" align="left" alt="Portal Solver Logo">

## [Portal Solver Editor SDK](../../pse_sdk.md) > [Reference](../index.md) > [Enums.h](index.md) > [PseEvent]()

<br>

| Значение (HEX) | Название                     | Описание                                                      | Разметка PseData для результата (PseResult)                                |
|:--------------:|------------------------------|---------------------------------------------------------------|----------------------------------------------------------------------------------|
| **0x00000000** | PSE_EVENT_ELEMENT_CHANGED    | Любое изменение элемента на карте                             | `[0]` **PseGUID** GUID<br>`[8]` **PseCallback** Callback<br>`[16]` **PseState** State |
| **0x00000001** | PSE_EVENT_GAME_TICK_OVERFLOW | Предупреждение при переизбытке SDK-команд за один игровой тик | **-**                                                                            |
| **0xFFFFFFFF** | PSE_EVENT_NONE               | Служебное пустое состояние                                    | **-**                                                                            |
