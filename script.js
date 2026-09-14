const size=25;
let maze,player,pathTaken=[],shortestPath=[];

const start={x:0,y:0};
const end={x:size-1,y:size-1};

function generateMaze(){

document.getElementById("result").style.display="none";
document.getElementById("fireworks").innerHTML="";

maze=Array.from({length:size},()=>Array(size).fill(1));

function carve(x,y){

let dirs=[[2,0],[-2,0],[0,2],[0,-2]]
.sort(()=>Math.random()-0.5);

for(let [dx,dy] of dirs){

let nx=x+dx;
let ny=y+dy;

if(nx>=0&&ny>=0&&nx<size&&ny<size&&maze[nx][ny]===1){

maze[x+dx/2][y+dy/2]=0;
maze[nx][ny]=0;
carve(nx,ny);
}
}
}

maze[0][0]=0;
carve(0,0);

for(let i=0;i<140;i++){
let x=Math.floor(Math.random()*size);
let y=Math.floor(Math.random()*size);

if(!(x===0&&y===0)&&!(x===end.x&&y===end.y)){
maze[x][y]=0;
}
}

maze[end.x][end.y]=0;

player={x:0,y:0};
pathTaken=[[0,0]];
shortestPath=bfs();

drawMaze();
}

function drawMaze(){

const box=document.getElementById("maze");
box.innerHTML="";
box.style.gridTemplateColumns=`repeat(${size},1fr)`;

for(let i=0;i<size;i++){
for(let j=0;j<size;j++){

let cell=document.createElement("div");
cell.classList.add("cell");

if(maze[i][j]===1) cell.classList.add("wall");
else cell.classList.add("path");

if(i===start.x&&j===start.y) cell.classList.add("start");
if(i===end.x&&j===end.y) cell.classList.add("end");
if(i===player.x&&j===player.y) cell.classList.add("player");

box.appendChild(cell);
}
}
}

function bfs(){

let q=[[0,0,[]]];
let vis=Array.from({length:size},()=>Array(size).fill(false));

while(q.length){

let [x,y,path]=q.shift();

if(x<0||y<0||x>=size||y>=size) continue;
if(vis[x][y]||maze[x][y]===1) continue;

vis[x][y]=true;
path=[...path,[x,y]];

if(x===end.x&&y===end.y) return path;

q.push([x+1,y,path]);
q.push([x-1,y,path]);
q.push([x,y+1,path]);
q.push([x,y-1,path]);
}

return [];
}

function movePlayer(dx,dy){

let nx=player.x+dx;
let ny=player.y+dy;

if(nx>=0&&ny>=0&&nx<size&&ny<size&&maze[nx][ny]===0){

player={x:nx,y:ny};
pathTaken.push([nx,ny]);
drawMaze();

if(player.x===end.x&&player.y===end.y){
winGame();
}
}
}

function winGame(){

let extra =
pathTaken.length === shortestPath.length
? "🔥 Perfect! You found shortest path!"
: `⚡ Difference: ${pathTaken.length-shortestPath.length} extra steps`;

document.getElementById("result").innerHTML=`
🎉 You Win!<br><br>
Your Path: ${pathTaken.length}<br>
Shortest Path: ${shortestPath.length}<br>
${extra}
`;

document.getElementById("result").style.display="block";

startFireworks();
}

function startFireworks(){

let area=document.getElementById("fireworks");

for(let t=0;t<12;t++){

setTimeout(()=>{

let cx=Math.random()*area.clientWidth;
let cy=Math.random()*area.clientHeight;

for(let i=0;i<30;i++){

let spark=document.createElement("div");
spark.className="spark";

let angle=Math.random()*360;
let dist=40+Math.random()*100;

spark.style.left=cx+"px";
spark.style.top=cy+"px";
spark.style.setProperty("--x",Math.cos(angle)*dist+"px");
spark.style.setProperty("--y",Math.sin(angle)*dist+"px");
spark.style.background=`hsl(${Math.random()*360},100%,60%)`;

area.appendChild(spark);

setTimeout(()=>spark.remove(),1000);
}

},t*300);
}
}

window.addEventListener("keydown",(e)=>{
if(e.key==="ArrowUp") movePlayer(-1,0);
if(e.key==="ArrowDown") movePlayer(1,0);
if(e.key==="ArrowLeft") movePlayer(0,-1);
if(e.key==="ArrowRight") movePlayer(0,1);
});

generateMaze();