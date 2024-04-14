// If a user sends a message, everyone in the chatroom should receive the message.

 

let sendBtn = document.querySelector('.send-btn')
let recipientName = document.querySelector('.recipient-name')
let recipientMsg = document.querySelector('.recipient-msg')
let newChatContainer = document.createElement('div')


function sendMessage() {

    // add contact details to all chats
    //addContactDetails()

    // display error if fields are empty
    if (recipientName.value === '' || recipientMsg.value === '') {
        alert('Fields must not be empty! ')
    } else {
        // render back to all chats page
        window.location.href = 'all-chats.html'
        // adds the contact name, pfp, message and time of message to the All Chats section
        newChatContainer.innerHTML = 
        `           
        <div class="chat-header">
            <h2>New Chat</h2>
            <button class="close-chat">Close</button>
        </div>

        <div class="chat-messages"></div>
        <input type="text" class="message-input" placeholder="Type a message...">
        <button class="send-message">Send</button>
        `

    }

}
sendBtn.addEventListener('click', sendMessage)



// call functions
sendMessage()