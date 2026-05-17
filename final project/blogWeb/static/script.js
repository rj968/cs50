// GETTING CURRENT DATE TIME FOR TIMESTAMP ELEMENT
const months = [
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
];
const days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];

function formatTimestamp() {
  const timestamp = new Date();
  const day = days[timestamp.getDay()];
  const month = months[timestamp.getMonth()];
  
  const date = `${day} ${timestamp.getDate()} ${month} ${timestamp.getFullYear()}`;
  const time = timestamp.toLocaleTimeString();
  
  return `
   <span class="date">${date}</span>
   <span class="time">${time}</span>
  `;
}
function updateTime() {
  document.getElementById("timestamp").innerHTML = formatTimestamp();
}

function startClock() {
  if (!document.getElementById("timestamp")) return;
  updateTime();
  setInterval(updateTime, 1000);
}
document.addEventListener("DOMContentLoaded",startClock)

// NAVBAR //
const navbar = document.getElementById("navbar");
const openButton = document.getElementById('open-sidebar-button')
const media=window.matchMedia("(width<600px)")

media.addEventListener('change',(e)=>updateNavbar(e))

function updateNavbar(e){
 const isMobile = e.matches;
 if(isMobile){
  navbar.setAttribute('inert','')
 }
 else{
  navbar.removeAttribute('inert')
 }
}

function openSidebar() {
 navbar.classList.add("show");
 openButton.setAttribute('aria-expanded','true');
 navbar.removeAttribute("inert");
}

function closeSidebar() {
 navbar.classList.remove("show");
 openButton.setAttribute("aria-expanded", "false");
 navbar.setAttribute("inert", "");
}

updateNavbar(media)

function deleteBlog(id) {
 if (!confirm(`Are you sure you want to delete this blog?`)) {
  return;
 }
 fetch("/delete_blog", {
  method: "POST",
  headers: {
   "Content-Type": "application/json"
  },
  body: JSON.stringify({ blog_id: id })
  })
 .then((response) => response.json())
 .then((data) => {
  if (data.success) {
   document.getElementById(id).parentElement.parentElement.remove();
  } else {
   alert("Failed to delete blog");
  }
 })
 .catch((err) => console.error(err));
}
