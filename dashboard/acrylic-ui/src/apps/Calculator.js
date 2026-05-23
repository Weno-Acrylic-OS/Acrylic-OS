import React, { useState } from 'react';
import './Calculator.css';

const Calculator = () => {
    const [display, setDisplay] = useState('0');
    const [currentValue, setCurrentValue] = useState(null);
    const [operator, setOperator] = useState(null);
    const [waitingForOperand, setWaitingForOperand] = useState(false);

    const inputDigit = (digit) => {
        if (waitingForOperand) {
            setDisplay(String(digit));
            setWaitingForOperand(false);
        } else {
            setDisplay(display === '0' ? String(digit) : display + digit);
        }
    };

    const inputDecimal = () => {
        if (waitingForOperand) {
            setDisplay('0.');
            setWaitingForOperand(false);
            return;
        }
        if (display.indexOf('.') === -1) {
            setDisplay(display + '.');
        }
    };

    const clearDisplay = () => {
        setDisplay('0');
        setCurrentValue(null);
        setOperator(null);
        setWaitingForOperand(false);
    };

    const performOperation = (nextOperator) => {
        const inputValue = parseFloat(display);

        if (currentValue == null) {
            setCurrentValue(inputValue);
        } else if (operator) {
            const result = calculate(currentValue, inputValue, operator);
            setCurrentValue(result);
            setDisplay(String(result));
        }

        setWaitingForOperand(true);
        setOperator(nextOperator);
    };

    const calculate = (left, right, op) => {
        switch (op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            default: return right;
        }
    };

    const handleEquals = () => {
        const inputValue = parseFloat(display);
        if (operator && currentValue !== null) {
            const result = calculate(currentValue, inputValue, operator);
            setCurrentValue(null); // Reset for next calculation
            setDisplay(String(result));
            setOperator(null);
            setWaitingForOperand(true);
        }
    };

    return (
        <div className="calculator">
            <div className="calculator-display">{display}</div>
            <div className="calculator-keypad">
                <div className="input-keys">
                    <div className="function-keys">
                        <button className="key-clear" onClick={clearDisplay}>AC</button>
                    </div>
                    <div className="digit-keys">
                        <button className="key-0" onClick={() => inputDigit(0)}>0</button>
                        <button className="key-dot" onClick={inputDecimal}>.</button>
                        <button onClick={() => inputDigit(1)}>1</button>
                        <button onClick={() => inputDigit(2)}>2</button>
                        <button onClick={() => inputDigit(3)}>3</button>
                        <button onClick={() => inputDigit(4)}>4</button>
                        <button onClick={() => inputDigit(5)}>5</button>
                        <button onClick={() => inputDigit(6)}>6</button>
                        <button onClick={() => inputDigit(7)}>7</button>
                        <button onClick={() => inputDigit(8)}>8</button>
                        <button onClick={() => inputDigit(9)}>9</button>
                    </div>
                </div>
                <div className="operator-keys">
                    <button onClick={() => performOperation('/')}>÷</button>
                    <button onClick={() => performOperation('*')}>×</button>
                    <button onClick={() => performOperation('-')}>−</button>
                    <button onClick={() => performOperation('+')}>+</button>
                    <button onClick={handleEquals}>=</button>
                </div>
            </div>
        </div>
    );
};

export default Calculator;
