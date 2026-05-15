const chatMessages = document.getElementById('chat-messages');
const userInput = document.getElementById('user-input');
const sendBtn = document.getElementById('send-btn');


const botRules = [
    {
        keywords: ["hi", "hello", "hey", "greetings"],
        response: "Hello! Welcome to our store. How can I assist you today?"
    },
    {
        keywords: ["shipping", "track", "delivery", "arrive"],
        response: "Standard shipping takes 3-5 business days. You can track your order using the link in your confirmation email."
    },
    {
        keywords: ["return", "refund", "exchange"],
        response: "We offer a 30-day return policy. Please ensure the item is unused and in its original packaging. Would you like a link to our return portal?"
    },
    {
        keywords: ["order", "status"],
        response: "To check your order status, please type your 6-digit order ID."
    },
    {
        keywords: ["human", "agent", "contact", "support", "help"],
        response: "I understand you'd like to speak to a person. Connecting you to the next available human agent... Please hold."
    },
    {
        // A simple regex rule example for checking 6 digit order IDs
        regex: /^\d{6}$/, 
        response: "Looking up order ID... Your order is currently out for delivery!"
    }
];

const fallbackResponse = "I'm sorry, I didn't quite understand that. You can ask me about shipping, returns, or order status.";


function getBotResponse(input) {
    const text = input.toLowerCase();

    for (let rule of botRules) {
        // Check standard keywords
        if (rule.keywords) {
            for (let word of rule.keywords) {
                if (text.includes(word)) {
                    return rule.response;
                }
            }
        }
        if (rule.regex && rule.regex.test(text.trim())) {
            return rule.response;
        }
    }
    
    return fallbackResponse;
}

function appendMessage(text, sender) {
    const msgDiv = document.createElement('div');
    msgDiv.classList.add('message');
    msgDiv.classList.add(sender === 'user' ? 'user-msg' : 'bot-msg');
    msgDiv.innerText = text;
    
    chatMessages.appendChild(msgDiv);
    
    chatMessages.scrollTop = chatMessages.scrollHeight;
}


function handleSend() {
    const text = userInput.value.trim();
    if (text === '') return;


    appendMessage(text, 'user');
    userInput.value = '';


    setTimeout(() => {
        const reply = getBotResponse(text);
        appendMessage(reply, 'bot');
    }, 500); 
}


sendBtn.addEventListener('click', handleSend);
userInput.addEventListener('keypress', function(e) {
    if (e.key === 'Enter') {
        handleSend();
    }
});


window.onload = () => {
    setTimeout(() => {
        appendMessage("Hi there! I'm the automated support assistant. How can I help?", 'bot');
    }, 500);
};