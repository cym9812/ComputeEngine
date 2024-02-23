import abc
from typing import Dict, Any, List


class BaseOp:

    @staticmethod
    def parse(data: Dict[str, Any]):
        query_type = data["type"]
        if query_type == "operation":
            return op_map[data["operation"]](**data)
        elif query_type == "variable":
            return Variable(data["value"])
        elif query_type == "value":
            return Value(data["value"])
        else:
            raise TypeError(f"Unknown query type: {query_type}")

    def _set_input_helper(self, op, operations):
        if len(operations) != 1:
            raise ValueError("Expected exactly one operation as input")
        if input_required(op):
            return operations[0]
        elif is_input_allowed(op):
            return op.set_input(operations)

        return op

    def _replace_variable_helper(self, op, variable_dict: Dict[str, Any]):
        if isinstance(op, Variable):
            return variable_dict[op.name]
        elif is_variable_allowed(op):
            return op.replace_variable(variable_dict)

        return op

    @abc.abstractmethod
    def build_query(self):
        raise NotImplementedError(f"No build_query method implemented for {self.__class__.__name__}")

    def set_input(self, operations):
        raise NotImplementedError(f"No set_input method implemented for {self.__class__.__name__}")

    def replace_variable(self, variable_dict: Dict[str, Any]):
        raise NotImplementedError(f"No replace_variable method implemented for {self.__class__.__name__}")


class Variable(BaseOp):
    def __init__(self, value):
        self._value = value

    @property
    def name(self):
        return self._value

    def build_query(self):
        return {
            "type": "variable",
            "value": self._value
        }

    def is_default(self):
        return self._value == "default"


class Value(BaseOp):
    def __init__(self, value):
        self._value = value

    @property
    def value(self):
        return self._value

    def build_query(self):
        return {
            "type": "value",
            "value": self._value
        }


class CompareOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._left = self.parse(kwargs.pop("left"))
        self._right = self.parse(kwargs.pop("right"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "left": self._left.build_query(),
            "right": self._right.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._left = self._left.replace_variable(variable_dict)
        self._right = self._right.replace_variable(variable_dict)
        return self


class MathOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._left = self.parse(kwargs.pop("left"))
        self._right = self.parse(kwargs.pop("right"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "left": self._left.build_query(),
            "right": self._right.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._left = self._left.replace_variable(variable_dict)
        self._right = self._right.replace_variable(variable_dict)
        return self


class AbsOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._value = self._value.replace_variable(variable_dict)
        return self


class LogicalOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._operands = [self.parse(operand) for operand in kwargs.pop("operands")]

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "operands": [operand.build_query() for operand in self._operands]
        }

    def set_input(self, operations: List[BaseOp]):
        self._operands = operations
        return self

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._operands = [operand.replace_variable(variable_dict) for operand in self._operands]
        return self


class CountOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))
        self._initial_value = self.parse(kwargs.pop("initial_value"))
        self._unit = self.parse(kwargs.pop("unit"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query(),
            "initial_value": self._initial_value.build_query(),
            "unit": self._unit.build_query()
        }

    def set_input(self, operations: List[BaseOp]):
        self._value = self._set_input_helper(self._value, operations)
        return self

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._value = self._replace_variable_helper(self._value, variable_dict)
        return self


class AggregateOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._value = self._replace_variable_helper(self._value, variable_dict)
        return self


class JumpOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))
        self._from = self.parse(kwargs.pop("from"))
        self._to = self.parse(kwargs.pop("to"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query(),
            "from": self._from.build_query(),
            "to": self._to.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._value = self._replace_variable_helper(self._value, variable_dict)
        return self


class TrendOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))
        self._from = self.parse(kwargs.pop("from"))
        self._to = self.parse(kwargs.pop("to"))
        self._duration = self.parse(kwargs.pop("duration"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query(),
            "from": self._from.build_query(),
            "to": self._to.build_query(),
            "duration": self._duration.build_query()
        }

    def replace_variable(self, variable_dict: Dict[str, Any]):
        self._value = self._replace_variable_helper(self._value, variable_dict)
        return self


class DurationOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))
        self._min_duration = self.parse(kwargs.pop("minDuration"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query(),
            "minDuration": self._min_duration.build_query()
        }

    def set_input(self, operations: List[BaseOp]):
        self._value = self._set_input_helper(self._value, operations)
        return self


class SelectOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query()
        }


class JudgeOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._value = self.parse(kwargs.pop("value"))

    def build_query(self):
        return {
            "type": "operation",
            "operation": self._operation,
            "value": self._value.build_query()
        }

    def set_input(self, operations: List[BaseOp]):
        self._value = self._set_input_helper(self._value, operations)
        return self

    def replace_variable(self, variable_map: Dict[str, Any]):
        self._value = self._replace_variable_helper(self._value, variable_map)
        return self


class DeclareOp(BaseOp):
    def __init__(self, operation: str, **kwargs):
        self._operation = operation
        self._left = self.parse(kwargs.pop("left"))
        self._right = self.parse(kwargs.pop("right"))

        self._variable_name = None

    @property
    def variable_name(self):
        if self._variable_name is None:
            if not isinstance(self._left, Variable):
                raise TypeError("Left side of declare operation must be a variable")
            self._variable_name = self._left.name

        return self._variable_name

    @property
    def variable_value(self):
        return self._right.build_query()

    def set_input(self, operations: List[BaseOp]):
        self._right = self._set_input_helper(self._right, operations)
        return self

    def replace_variable(self, variable_map: Dict[str, Any]):
        self._right = self._replace_variable_helper(self._right, variable_map)
        return self

    def build_query(self):
        return self._right.build_query()


FinalResult = (DeclareOp, JudgeOp)
AllowInput = (LogicalOp, JudgeOp, CountOp, DeclareOp, DurationOp)
AllowVariable = (CompareOp, MathOp, AbsOp, LogicalOp, CountOp, AggregateOp, JudgeOp, TrendOp, JudgeOp, DeclareOp)

op_map = {
    "EQ": CompareOp,
    "NE": CompareOp,
    "GT": CompareOp,
    "LT": CompareOp,
    "GE": CompareOp,
    "LE": CompareOp,
    "ADD": MathOp,
    "SUB": MathOp,
    "MUL": MathOp,
    "DIV": MathOp,
    "POW": MathOp,
    "ABS": AbsOp,
    "AND": LogicalOp,
    "OR": LogicalOp,
    "COUNT": CountOp,
    "MAX": AggregateOp,
    "MIN": AggregateOp,
    "AVG": AggregateOp,
    "JUMP": JumpOp,
    "BEFORE": TrendOp,
    "AFTER": TrendOp,
    "HOLD": TrendOp,
    "DURATION": DurationOp,
    "SELECT": SelectOp,
    "JUDGE": JudgeOp,
    "DECLARE": DeclareOp,
}


def input_required(op: BaseOp) -> bool:
    return isinstance(op, Variable) and op.is_default()


def is_input_allowed(op: BaseOp) -> bool:
    return isinstance(op, AllowInput)


def is_variable_allowed(op: BaseOp) -> bool:
    return isinstance(op, AllowVariable)


def is_final_result(op: BaseOp) -> bool:
    return isinstance(op, FinalResult)


def build_operation(data: Dict[str, Any]):
    return BaseOp.parse(data)
