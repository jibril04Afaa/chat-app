let fullName = document.querySelector('.name-input')
let email = document.querySelector('.email-input')
let password = document.querySelector('.password-input')

let signupBtn = document.querySelector('.login-btn')


// Check sign up credentials
function checkLoginCreds() {
    // display error if fields are empty
    if (fullName.value === '' || email.value === '' || password.value === '') {
        alert('Fields must not be empty! ')
    } else {
        // render HTML new chat page

    }
}
signupBtn.addEventListener('click', checkLoginCreds)

// render login page if email exists

function userNameExists() {
    if () {

    } else {

    }
}