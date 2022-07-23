const textbox = document.getElementById('textbox')
const colors = ['red', 'blue', 'green', 'yellow', 'orange', 'purple']
const framesPerSecond = 30
let frame = 0
let bytes = null

function onTextboxEnter() {
    let content = textbox.value.trim()
    bytes = null
    fetch('/battle_dumps/' + content).then(
        function(response) {
            response.arrayBuffer().then(function(data) {
                console.log('recieved data ' + data)
                bytes = data
            })
        }
    )

    let metadata = null
    fetch('battle_metadata.json').then(
        function(response) {
            response.json().then(function(data) {
                console.log('recieved metadata' + data)
                metadata = data
            })
        }
    )

    metadata['fighters']

    frame = 0
    textbox.value = ''
}

function drawFrame() {

}

textbox.addEventListener('keydown', (e) => {
    if (e.key == 'Enter') {
        onTextboxEnter()
    }
});

window.setInterval(function () {

    frame++
}, 1000 / framesPerSecond);