import React, { useState } from 'react';

function CodeArea() {
  const [textBox1Value, setTextBox1Value] = useState<string>('');
  const [textBox2Value, setTextBox2Value] = useState<string>('');

  const handleTextBox1Change = (event: React.ChangeEvent<HTMLInputElement>) => {
    setTextBox1Value(event.target.value);
  };
  const translateText = () => {
    // Translate the text from textBox1Value here
    // For now, let's just set textBox2Value to the same as textBox1Value
    //setTextBox2Value(textBox1Value);
  };

  return (
    <div style={{ display: 'flex', height: '80vh' }}>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Cpp Code</h1>
        <textarea
          placeholder="Enter your text here..."
          value={textBox1Value}
          onChange={handleTextBox1Change}
          style={{ width: '100%', height: '88%', resize: 'none', boxSizing: 'border-box' }}
        />
      </div>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Go Code</h1>
        <textarea
          placeholder=""
          value={textBox2Value}
          readOnly
          style={{ width: '100%', height: '88%', resize: 'none', boxSizing: 'border-box' }}
        />
      </div>
      <div style={{ position: 'absolute', bottom: '20px', left: '50%', transform: 'translateX(-50%)' }}>
        <button onClick={translateText}>Translate</button>
      </div>
    </div>
  );
}

export default CodeArea;
