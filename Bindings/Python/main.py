import pse # Portal Solver Editor (PSE)

# Инициализируем PSE (yeah, dynamic library yo)
pse.initialize("../../Dist/bin/PSE.dll")

# Генерация процедурного окружения:
# Создаем шахматное поле 10x10 из динамических мешей
for x in range(10):
    for y in range(10):
        # Логика выбора материала в зависимости от позиции (шахматка)
        material = pse.Material.FLOOR_BLACK if (x + y) % 2 == 0 else pse.Material.FLOOR_WHITE

        # Создаем обьект "динамический меш"
        mesh = pse.DynamicMesh()
        mesh.create(
            mesh=pse.Mesh.PLANE,
            material=material,
            location=(x * 100 - 500, y * 100 - 500, 0)
        )

# Создаем элемент головоломки - дверь
door = pse.DoorElement()
door.create(location=(-100, 0, 0))

# И кнопку
button = pse.ButtonElement()
button.create(location=(0, 0, 0))

# Реактивное программирование зашло в чат
# Cвязываем событие изменения состояния кнопки (нажата/отпущена)
# С состоянием двери (открыть/закрыть)
@button.on_element_changed()
def my_super_cool_function_you_have_ever_seen(val):
    door.set_state(val)
    print(f"New val: {val}")

import ctypes

data = pse.BufferData()
pse.globals.dll.pseBufferDataCreateCommand(ctypes.byref(data), ctypes.c_uint32(0x05062025))
pse.globals.dll.pseCommandBufferSinglePushAndWait(ctypes.byref(data))

# Главный цикл обработки событий
# Метод poll_events опрашивает очередь событий вызывает нужные калбеки
while True:
    pse.poll_events()
