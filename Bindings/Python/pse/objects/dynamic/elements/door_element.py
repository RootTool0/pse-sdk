from . import _base_element

class DoorElement(_base_element.BaseElement):
    def _get_class(self) -> _base_element.Element: return _base_element.Element.DOOR
