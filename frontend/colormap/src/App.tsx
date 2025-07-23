import { useState } from 'react'
import './App.css'
import ImageUpload from './components/ImageUpload'


function App() {
  const colors = [
    '#FF0000', '#00FF00', '#0000FF', '#FFFF00', '#FF00FF', '#00FFFF',
    '#800000', '#008000', '#000080', '#808000', '#800080', '#008080'
  ]

  return (
    <>
      <div>
        <h1>Upload image to map it</h1>
        <ImageUpload />
        
      </div>
    </>
  )
}

export default App
