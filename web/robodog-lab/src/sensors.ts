// Practice inputs only. Coordinates: X right, Y up, Z toward Scout's nose.
export const inputs = { ax: 0, ay: 9.81, az: 0, gx: 0, gy: 0, gz: 0, temperature: 30 };
type Key = keyof typeof inputs;
const fields: [Key, string, number, number, number, string][] = [
  ['ax','X · sideways acceleration',-15,15,.1,'m/s²'],
  ['ay','Y · upward acceleration (includes gravity)',0,20,.01,'m/s²'],
  ['az','Z · forward acceleration',-15,15,.1,'m/s²'],
  ['gx','X · pitch rate',-90,90,1,'°/s'],
  ['gy','Y · turn rate',-90,90,1,'°/s'],
  ['gz','Z · roll rate',-90,90,1,'°/s'],
  ['temperature','Chip temperature',15,50,.5,'°C'],
];
export function mountSensors() {
  document.querySelector('aside')!.innerHTML = `<p class="eyebrow">SENSOR PLAYGROUND</p><h2>Make Scout react.</h2><p>Drag the dog to orbit the camera. Scroll to zoom. Try one sensor at a time.</p>
  <details open><summary>1. Accelerometer · push and bounce</summary><p>Gravity is included. At rest, Y reads about 9.81. Sideways pushes make Scout lean; upward pushes make him bounce. These are animations, not measured travel.</p><div id="accel-inputs"></div></details>
  <details><summary>2. Gyroscope · tumble and turn</summary><p>Choose a rotation speed. Scout keeps turning until you return it to zero. X pitches, Y turns, Z rolls.</p><div id="gyro-inputs"></div></details>
  <details><summary>3. Temperature · comfort acting</summary><p>This sensor measures its chip, not the room. Below 25°C Scout shivers; above 38°C he pants. These playful thresholds are not safety limits.</p><div id="temp-inputs"></div></details>
  <button class="reset" id="sensor-reset">Reset all sensors and pose</button><p id="reaction" role="status">Comfortable · ready to explore</p>
  <div class="connection"><strong>Practice inputs · no live sensor data</strong><small>No USB connection. No physical motors controlled.</small></div>`;
  for(const [key,label,min,max,step,unit] of fields) {
    const group=key.startsWith('a')?'accel':key.startsWith('g')?'gyro':'temp';
    document.querySelector('#'+group+'-inputs')!.insertAdjacentHTML('beforeend',`<label for="${key}">${label}<output id="${key}-read">${inputs[key]} ${unit}</output></label><input id="${key}" type="range" min="${min}" max="${max}" step="${step}" value="${inputs[key]}">`);
    document.querySelector<HTMLInputElement>('#'+key)!.addEventListener('input',e=>{
      inputs[key]=Number((e.target as HTMLInputElement).value);
      document.querySelector('#'+key+'-read')!.textContent=inputs[key]+' '+unit;
    });
  }
  return (resetPose:()=>void)=>document.querySelector('#sensor-reset')!.addEventListener('click',()=>{
    Object.assign(inputs,{ax:0,ay:9.81,az:0,gx:0,gy:0,gz:0,temperature:30});
    for(const [key,,,,,unit] of fields){document.querySelector<HTMLInputElement>('#'+key)!.value=String(inputs[key]);document.querySelector('#'+key+'-read')!.textContent=inputs[key]+' '+unit;}
    resetPose();
  });
}
