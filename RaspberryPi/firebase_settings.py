import pyrebase

firebaseConfig = {
  "apiKey": "AIzaSyCh1-JNdyucFCmrHGrtN_pPxbvZcjM_7ks",
  "authDomain": "eco-arcade.firebaseapp.com",
  "databaseURL": "https://eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app/",
  "projectId": "eco-arcade",
  "storageBucket": "eco-arcade.appspot.com",
  "messagingSenderId": "603676636472",
  "appId": "1:603676636472:web:ee78d1f2904e2548e13dcd",
  "measurementId": "G-X9XPHYEPRJ"
}

firebase = pyrebase.initialize_app(firebaseConfig)

def db():
  return firebase.database()