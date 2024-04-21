// If a user sends a message, everyone in the chatroom should receive the message.

// data from all chat rooms page
let chatParentContainer = document.querySelector('.display-chats-container') // from all-chats.html
let sendBtn = document.querySelector('.send-btn')
let recipientName = document.querySelector('.recipient-name')
let recipientMsg = document.querySelector('.recipient-msg')
let chatRoomName = document.querySelector('.chat-room-name')

function sendMessage() {
    // display error if fields are empty
    if (recipientName.value === '' || recipientMsg.value === '' || chatRoomName.value === '') {
        alert('Fields must not be empty! ')
    } else {
        // render back to all chats page
        window.location.href = 'all-chats.html'
        // generates a new chat room
        generateRoom("chat room 2", "hello from chat room 2")
    }
}

sendBtn.addEventListener('click', sendMessage)


// data from new chats page
let pfp = document.querySelector('.profile-pic')
let individualChatContainer = document.querySelector('.individual-chat-container')


// once the send button is clicked, a new chat room is generated

let newPrevMsg = document.createElement('div')
let newchatRoomName = document.createElement('div')

newPrevMsg.innerHTML = 'this is a new previous message'
chatParentContainer.appendChild(newPrevMsg)

if (newPrevMsg) {
  alert("created")
}


// generates new chat room
// the function arguments are the new-chat-room input boxes
function generateRoom(newchatRoomName, newPrevMsg) {
    

    // add new elements as children to the parent Node '.display-chats-container'


    // 1 - create a new element
    /* 2 - set the data entered from create-new-chat 
    to be in the chat room container with their 
    respective elements
    
  
    notes
    - newChatRoom serves as the parent element for the chatRoomName, newPfp
    and prevMsg elements
    */
  
    
}
  

// call functions
