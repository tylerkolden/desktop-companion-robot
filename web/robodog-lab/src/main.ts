import * as THREE from 'three';
import { RoundedBoxGeometry } from 'three/addons/geometries/RoundedBoxGeometry.js';
import './style.css';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { inputs, mountSensors } from './sensors';
document.querySelector('#app')!.innerHTML=`<header><b>andrew's / ROBOTICS CLUB</b><span class="badge">● PRACTICE MODE</span></header><main><section class="intro"><p class="eyebrow">EXPERIMENT 08 · THE DIGITAL DOG</p><h1>A little code.<br>A lot of <em>personality.</em></h1><p class="lede">Meet Scout. Tilt his world, change his mood, and discover how a robot learns to move.</p></section><section class="workspace"><div class="stage"><div class="stage-top"><span>SCOUT / 001</span><span>3D PLAYGROUND</span></div><div id="scene" aria-label="Animated 3D robot dog"></div><div class="stage-bottom"><span id="caption">Curious ears. Big ideas.</span><button id="pause">Pause animation</button></div></div><aside><p class="eyebrow">YOUR MISSION</p><h2>Give Scout a little lean.</h2><p>Move a slider. Watch his body follow. These controls pretend to be the motion sensor.</p><label for="roll">Lean left / right <output id="roll-value">0°</output></label><input id="roll" type="range" min="-25" max="25" value="0"><label for="pitch">Nose down / up <output id="pitch-value">0°</output></label><input id="pitch" type="range" min="-20" max="20" value="0"><button id="reset" class="reset">Back to center ↗</button><div class="lesson"><strong>Why does he lean?</strong><p>A motion sensor measures acceleration and rotation rate. Software can estimate tilt. Here, you supply the tilt.</p></div><div class="connection"><strong>Simulated input</strong><small>No USB connection. No motors controlled.</small></div></aside></section><section class="expressions"><div><p class="eyebrow">A FACE FOR EVERY FEELING</p><h2>Pick a personality.</h2></div><div class="moods" role="group" aria-label="Dog expression">${['Curious','Happy','Sleepy','Cool'].map((m,i)=>`<button data-mood="${m}" aria-pressed="${i===0}">${m}</button>`).join('')}</div></section><footer><span>Built for Andrew. Powered by curiosity.</span><span>Browser demo · Sensor connection comes next</span></footer></main>`;
const host=document.querySelector<HTMLDivElement>('#scene')!;
const scene=new THREE.Scene();scene.background=new THREE.Color('#e9efeb');
const camera=new THREE.PerspectiveCamera(35,1,.1,100);camera.position.set(4.5,3.2,6.5);camera.lookAt(0,.8,0);
let renderer:THREE.WebGLRenderer;
try{renderer=new THREE.WebGLRenderer({antialias:true});}catch{host.textContent='3D graphics could not start. Try Chrome or Edge with graphics acceleration enabled.';throw new Error('WebGL unavailable');}
renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.shadowMap.enabled=true;renderer.shadowMap.type=THREE.PCFSoftShadowMap;host.append(renderer.domElement);
scene.add(new THREE.HemisphereLight(0xffffff,0x718879,1.8));const sun=new THREE.DirectionalLight(0xffffff,2);sun.position.set(3,7,5);sun.castShadow=true;sun.shadow.mapSize.set(2048,2048);scene.add(sun);
const mat=(color:string)=>new THREE.MeshStandardMaterial({color,roughness:.6});const cream=mat('#fff5dc'),orange=mat('#f07545'),dark=mat('#193c36'),joint=mat('#667e72');
function box(w:number,h:number,d:number,m:THREE.Material,parent:THREE.Object3D,x=0,y=0,z=0,r=.12){const o=new THREE.Mesh(new RoundedBoxGeometry(w,h,d,4,r),m);o.position.set(x,y,z);o.castShadow=true;o.receiveShadow=true;parent.add(o);return o;}
const floor=new THREE.Mesh(new THREE.CylinderGeometry(2.4,2.5,.16,80),mat('#d3dfd6'));floor.position.y=-.09;floor.receiveShadow=true;scene.add(floor);
const dog=new THREE.Group();scene.add(dog);box(1.3,.85,1.65,cream,dog,0,1.05,0);box(1.34,.2,1.4,orange,dog,0,1.12,0);
for(const x of [-.48,.48])for(const z of [-.55,.55]){box(.34,.65,.38,joint,dog,x,.55,z,.08);box(.48,.25,.6,cream,dog,x,.18,z+.08,.09);}
const head=new THREE.Group();head.position.set(0,1.75,.72);dog.add(head);box(1.65,1.12,.95,cream,head);box(1.42,.88,.08,dark,head,0,0,.5);
const ears:THREE.Mesh[]=[];for(const s of [-1,1])ears.push(box(.38,.9,.55,orange,head,s*.91,-.12,-.08));
const faceCanvas=document.createElement('canvas');faceCanvas.width=512;faceCanvas.height=256;const ctx=faceCanvas.getContext('2d')!;const texture=new THREE.CanvasTexture(faceCanvas);texture.colorSpace=THREE.SRGBColorSpace;const face=new THREE.Mesh(new THREE.PlaneGeometry(1.29,.68),new THREE.MeshBasicMaterial({map:texture}));face.position.set(0,0,.549);head.add(face);
const tail=new THREE.Group();tail.position.set(0,1.25,-.85);dog.add(tail);box(.22,.65,.24,orange,tail,0,.25,-.12,.09).rotation.x=-.6;
let mood='Curious',paused=matchMedia('(prefers-reduced-motion: reduce)').matches,time=0,last=performance.now();
function drawFace(t:number){ctx.fillStyle='#193c36';ctx.fillRect(0,0,512,256);ctx.strokeStyle=ctx.fillStyle='#bcf4c7';ctx.lineWidth=12;ctx.lineCap='round';const blink=mood==='Sleepy'||t%4.7>4.48;for(const x of [150,362]){if(blink){ctx.beginPath();ctx.moveTo(x-37,100);ctx.lineTo(x+37,100);ctx.stroke();}else if(mood==='Cool'){ctx.fillStyle='#f6b46c';ctx.fillRect(x-57,55,108,79);ctx.fillStyle='#193c36';ctx.fillRect(x-41,65,30,14);ctx.fillStyle='#bcf4c7';}else{ctx.beginPath();ctx.ellipse(x+Math.sin(t*.7)*10,94,30,mood==='Happy'?30:43,0,0,Math.PI*2);ctx.fill();}}if(mood==='Cool'){ctx.strokeStyle='#f6b46c';ctx.beginPath();ctx.moveTo(200,75);ctx.lineTo(310,75);ctx.stroke();}ctx.strokeStyle='#bcf4c7';ctx.beginPath();if(mood==='Sleepy')ctx.ellipse(256,187,17,12,0,0,Math.PI*2);else{ctx.moveTo(215,174);ctx.quadraticCurveTo(256,mood==='Happy'?233:208,297,174);}ctx.stroke();if(inputs.temperature>38){ctx.fillStyle='#f09585';ctx.beginPath();ctx.roundRect(247,185,28,44,12);ctx.fill();}texture.needsUpdate=true;}
const bindReset=mountSensors();
const bounds=new THREE.Box3();
const orientation=new THREE.Quaternion();
const deltaRotation=new THREE.Quaternion();
const rotationVector=new THREE.Vector3();
const controls=new OrbitControls(camera,renderer.domElement);
controls.target.set(0,.8,0);controls.enableDamping=true;controls.minDistance=4;controls.maxDistance=12;controls.enablePan=false;controls.update();
bindReset(()=>{orientation.identity();dog.quaternion.identity();dog.position.set(0,0,0);});
const pause=document.querySelector<HTMLButtonElement>('#pause')!;if(paused)pause.textContent='Resume animation';pause.addEventListener('click',()=>{paused=!paused;pause.textContent=paused?'Resume animation':'Pause animation';});
for(const b of document.querySelectorAll<HTMLButtonElement>('[data-mood]'))b.addEventListener('click',()=>{mood=b.dataset.mood!;for(const a of document.querySelectorAll('[data-mood]'))a.setAttribute('aria-pressed',String(a===b));document.querySelector('#caption')!.textContent=({Curious:'Curious ears. Big ideas.',Happy:'Good dog. Great experiment.',Sleepy:'Recharging his imagination.',Cool:'Certified good boy. Very cool.'} as Record<string,string>)[mood];drawFace(time);});
new ResizeObserver(()=>{camera.aspect=host.clientWidth/host.clientHeight;camera.updateProjectionMatrix();renderer.setSize(host.clientWidth,host.clientHeight);}).observe(host);
function frame(now:number){
 const dt=Math.min((now-last)/1000,.05);last=now;
 if(!paused){
  time+=dt;
  rotationVector.set(inputs.gx,inputs.gy,inputs.gz).multiplyScalar(Math.PI/180);
  const speed=rotationVector.length();
  if(speed>0){deltaRotation.setFromAxisAngle(rotationVector.normalize(),speed*dt);orientation.multiply(deltaRotation).normalize();}
  const lean=new THREE.Quaternion().setFromEuler(new THREE.Euler(THREE.MathUtils.clamp(inputs.az/35,-.35,.35),0,THREE.MathUtils.clamp(-inputs.ax/35,-.35,.35)));
  dog.quaternion.slerp(orientation.clone().multiply(lean),1-Math.exp(-8*dt));
  const cold=inputs.temperature<25,hot=inputs.temperature>38;
  dog.position.x=THREE.MathUtils.damp(dog.position.x,inputs.ax*.015,8,dt)+(cold?Math.sin(time*35)*.003:0);
  dog.position.z=THREE.MathUtils.damp(dog.position.z,inputs.az*.015,8,dt);
  dog.position.y=Math.max(0,(inputs.ay-9.81)*.035)*(0.5+0.5*Math.sin(time*7));
  head.rotation.z=mood==='Curious'?Math.sin(time*.65)*.09:0;
  head.position.y=1.75+Math.sin(time*(hot?7:1.7))*(hot?.045:.025);
  tail.rotation.z=Math.sin(time*(mood==='Happy'?10:3))*(mood==='Sleepy'?0:.35);
  ears.forEach((e,i)=>e.rotation.z=(i===0?-1:1)*(.2+Math.sin(time*1.8)*.035));
  dog.updateMatrixWorld(true);bounds.setFromObject(dog);
  dog.position.y+=Math.max(0,.02-bounds.min.y);
 }
 const reaction=inputs.temperature<25?'Chilly acting - shiver':inputs.temperature>38?'Warm acting - pant':'Comfortable - curious';
 const status=document.querySelector('#reaction')!;
 const message=paused?'Paused - change inputs, then resume':reaction;
 if(status.textContent!==message)status.textContent=message;
 drawFace(time);controls.update();renderer.render(scene,camera);requestAnimationFrame(frame);
}requestAnimationFrame(frame);
