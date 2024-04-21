// If a user sends a message, everyone in the chatroom should receive the message.

 
let createChatBtn = document.querySelector('.create-new-chat')

// create a new chat
createChatBtn.addEventListener('click', function() {
    // render HTML new-chat page
    window.location.href = 'new-chat.html'
    
})

