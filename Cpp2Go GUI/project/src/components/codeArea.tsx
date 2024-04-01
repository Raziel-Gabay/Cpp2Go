import React, { useState } from 'react';
import { UnControlled as CodeMirror } from 'react-codemirror2';
import 'codemirror/lib/codemirror.css';
import 'codemirror/mode/clike/clike';
import 'codemirror/mode/go/go';

function CodeArea() {
  const [cppCode, setCppCode] = useState('');
  const [goCode, setGoCode] = useState('');

  const translateText = async () => {
    console.log('translating text');
    const result = await fetch("http://127.0.0.1:8080", { method: "POST", body: cppCode });
    const body = await result.text();
    setGoCode(body);
  };

  return (
    <div style={{ display: 'flex', height: '80vh' }}>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Cpp Code</h1>
          <CodeMirror
            value={cppCode}
            options={{
              mode: 'text/x-c++src',
              theme: 'default',
              lineNumbers: true,
            }}
            onChange={(editor, data, value) => {setCppCode(value)}} />
      </div>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Go Code</h1>
              <CodeMirror 
              value={goCode} 
              options={{ 
                readOnly: true,
                mode: 'go',
                theme: 'default',
                lineNumbers: true,
                }} />
      </div>
      <div style={{ position: 'absolute', bottom: '20px', left: '50%', transform: 'translateX(-50%)' }}>
        <button onClick={translateText}>Translate</button>
      </div>
    </div>
  );
}

export default CodeArea;
