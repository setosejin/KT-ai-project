//import logo from './logo.svg';
import './App.css';

function App() {

  var video = document.querySelector("#videoElement");
 
  if (navigator.mediaDevices.getUserMedia) {
    navigator.mediaDevices.getUserMedia({ video: true })
      .then(function (stream) {
        video.srcObject = stream;
      })
      .catch(function (err0r) {
        console.log("Something went wrong!");
      });
  }

  return (
    <div className="App">
      <header className="App-header">
        
        <ul class="nav">
          <li class="nav-item">
            <a class="nav-link active" aria-current="page" href="alarm">Alarm</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" href="clothes">Clothes</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" href="mask">Mask</a>
          </li>
          
        </ul>
      </header>

      <body className="App-body">
        <div id="container">
          <video autoplay="true" id="videoElement">
      
          </video>
        </div>
      </body>
    </div>
  );
}

export default App;
