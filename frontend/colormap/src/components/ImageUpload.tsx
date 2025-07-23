import { useState } from 'react'
import '../App.css'
function ImageUpload() {
  const [image, setImage] = useState<string | null>(null)

  const handleImageChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0]
    if (file) {
      const reader = new FileReader()
      reader.onloadend = () => {
        setImage(reader.result as string)
      }
      reader.readAsDataURL(file)
    }
  }

  return (
    <div className='image-upload'>
      <input type="file" accept="image/*" onChange={handleImageChange} />
      {image && <img src={image} alt="Uploaded" style={{ maxWidth: '100%' }} />}
    </div>
  )
}
export default ImageUpload