const table = document.getElementsByClassName('tic')[0];
const incorrect = table.getElementsByClassName('incorrect');
const correct = document.getElementById('correct');
const cell00 = table.rows[0].cells[0];
const cell11 = table.rows[1].cells[1];

for (let i = 0; i < incorrect.length; i++) {
    incorrect[i].addEventListener('click', function() {
        alert("Incorrect answer");
        incorrect[i].style.backgroundColor = 'red';
        incorrect[i].innerHTML = ':\(';
    })
}

correct.addEventListener('click', function() {
    alert("correct answer");
    correct.style.backgroundColor = 'green';
    correct.innerHTML = 'X';
    cell00.style.backgroundColor = 'green';
    cell11.style.backgroundColor = 'green';
})
