import React, { useState, useEffect, useRef } from 'react';
import { EditorState, EditorView, basicSetup } from '@codemirror/basic-setup';
import { cpp } from '@codemirror/lang-cpp';
import { spawn } from 'child_process';
import { saveAs } from 'file-saver';

function CodeArea() {
  const [textBox1Value] = useState<string>('');
  const [textBox2Value, setTextBox2Value] = useState<string>('');

  const codeContainer1Ref = useRef<HTMLDivElement | null>(null);
  const codeContainer2Ref = useRef<HTMLDivElement | null>(null);
  const editorView1Ref = useRef<EditorView | null>(null); // Ref for the first EditorView
  const editorView2Ref = useRef<EditorView | null>(null); // Ref for the second EditorView

  useEffect(() => {
    if (codeContainer1Ref.current && !editorView1Ref.current) {
      const editorState = EditorState.create({
        doc: textBox1Value,
        extensions: [basicSetup]
      });
      editorView1Ref.current = new EditorView({
        state: editorState,
        parent: codeContainer1Ref.current
      });
    }

    if (codeContainer2Ref.current && !editorView2Ref.current) {
      const editorState = EditorState.create({
        doc: textBox2Value,
        extensions: [basicSetup, EditorState.readOnly.of(true)] // Add readOnly extension
      });
      editorView2Ref.current = new EditorView({
        state: editorState,
        parent: codeContainer2Ref.current
      });
    }

    return () => {
      if (editorView1Ref.current) {
        editorView1Ref.current.destroy();
        editorView1Ref.current = null;
      }

      if (editorView2Ref.current) {
        editorView2Ref.current.destroy();
        editorView2Ref.current = null;
      }
    };
  }, [textBox1Value, textBox2Value]);




  const translateText = async () => {
    // Save content of textBox1Value to a text file
    console.log('translating text');
    const content = editorView1Ref.current?.state.doc.toString() || '';
    const result = await fetch("127.0.0.1:8080", { method: "POST", body: JSON.stringify({ code: content }) })
    const body = await result.json()
    setTextBox2Value(body.code);
    //const blob = new Blob([content], { type: 'text/plain;charset=utf-8' });
    //saveAs(blob, 'cppCode.txt');
    setTextBox2Value("sadasd");
  };
  return (
    <div style={{ display: 'flex', height: '80vh' }}>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Cpp Code</h1>
        <div ref={codeContainer1Ref} className="code-container" style={{ width: '100%', height: '88%', resize: 'none', boxSizing: 'border-box', border: '1px solid #ccc', padding: '5px', overflowY: 'auto' }}>
          {textBox1Value}
        </div>
      </div>
      <div style={{ flex: 1, padding: '20px', height: '100%' }}>
        <h1>Go Code</h1>
        <div ref={codeContainer2Ref} className="code-container" style={{ width: '100%', height: '88%', resize: 'none', boxSizing: 'border-box', border: '1px solid #ccc', padding: '5px', overflowY: 'auto' }}>
          {textBox2Value}
        </div>
      </div>
      <div style={{ position: 'absolute', bottom: '20px', left: '50%', transform: 'translateX(-50%)' }}>
        <button onClick={translateText}>Translate</button>
      </div>
    </div>
  );
}

export default CodeArea;
