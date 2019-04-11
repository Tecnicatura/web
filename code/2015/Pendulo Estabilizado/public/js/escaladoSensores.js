
function fit(in0,from0,to0,fromFix0,toFix0){

var a1=from0;
var a2=to0;
var b1=1;
var b2=1;
var c1=fromFix0;
var c2=toFix0;
var den=(a2*b1-a1*b2);
var res=0;

if(den!=0){
	//a1x+b1y=c1
	//a2x+b2y=c2
	//cramer
	var x=(b1*c2-b2*c1)/den;
	var y=(a2*c1-a1*c2)/den;
	res=in0*x+y;

}else{

	console.log("error div 0");
}
return res;
}//fit(5, 1,100,1,100)




function tablaVeleta(anVeleta){

	var texto;

	if(anVeleta>195){
		texto= "Oeste";
	}

	if(anVeleta>128 && anVeleta<=195){
		texto= "Nor-Oeste";
	}

	if(anVeleta>75 && anVeleta<=128){
		texto= "Norte";
	}

	if(anVeleta>39 && anVeleta<=75){
		texto= "Sur-Oeste";
	}

	if(anVeleta>21 && anVeleta<=39){
		texto= "Nor-Este";
	}

	if(anVeleta>10 && anVeleta<=21){
		texto= "Sur";
	}

	if(anVeleta>5 && anVeleta<=10){
		texto= "Sur-Este";
	}

	if(anVeleta>=0 && anVeleta<=5){
		texto= "Este";
	}

	texto= '<h1>'+texto+'</h1>'
	return texto;
}




function tablaLluvia(anLluvia){

	var texto;

	if(anLluvia>=0 && anLluvia<=76.5){
		texto= "Intensa";
	}

	if(anLluvia>76.5 && anLluvia<=102){
		texto= "Moderada";
	}

	if(anLluvia>102 && anLluvia<=155){
		texto= "Llovizna";
	}

	if(anLluvia>155 && anLluvia<=178.5){
		texto= "Sin lluvia";
	}

	if(anLluvia>178.5){
		texto= "Sin lluvia";
	}

	texto= '<h1>'+texto+'</h1>'
	return texto;
}


function tope100(arg) {
	if(arg>100) return 100;
	if(arg<0) return 0
	return arg;
}



var escala= { "t":{ "minVal":0, "maxVal":10000},
              "e":{ "minVal":-200, "maxVal":200, "horizontalLines":[{color:'#000000',lineWidth:1,value:0},{color:'#880000',lineWidth:1,value:164},{color:'#880000',lineWidth:1,value:-164}]},
              "c":{ "minVal":-1500, "maxVal":1500}, //"horizontalLines":[{color:'#000000',lineWidth:1,value:0},{color:'#880000',lineWidth:1,value:33},{color:'#880000',lineWidth:1,value:-33}]},
              "p":{ "minVal":0, "maxVal":1024},
              "Kp":{ "minVal":0, "maxVal":20},
              "Ki":{ "minVal":0, "maxVal":20},
              "Kd":{ "minVal":0, "maxVal":20},
              "Kp2":{ "minVal":0, "maxVal":20},
              "Ki2":{ "minVal":0, "maxVal":20},
              "Kd2":{ "minVal":0, "maxVal":20}
       };


function escalado(datos){

/*var anT=datos['t'];*/
var anE=datos['e'];
var anC=datos['c'];
var anPwm=datos['pwm'];
/*var anKp=datos['Kp'];
var anKi=datos['Ki'];
var anKd=datos['Kd'];
var anKp2=datos['Kp2'];
var anKi2=datos['Ki2'];
var anKd2=datos['Kd2'];*/

//var anTempEscalado= fit(anTemp1-anTemp2/*valor*/,-136/*inicio*/,90/*fin*/, -52/*escala salida inicio*/,52/*escala salida fin*/);
//var anHumedadSueloEscalado=tope100( fit(anHumedadSuelo/*valor*/,25/*inicio*/,240/*fin*/, 100/*escala salida inicio*/,0/*escala salida fin*/));
//var humedadAmbienteEscalado= tope100( fit(humedadAmbiente/*valor*/,2*6033/*inicio*/,2*7351/*fin*/, 100/*escala salida inicio*/,0/*escala salida fin*/));
//var anemometroEscalado= fit(anemometro/*valor*/,0/*inicio*/,10/*fin*/, 0/*escala salida inicio*/,100/*escala salida fin*/);
//var anLluviaTabla=tablaLluvia(anLluvia);
//var anVeletaTabla=tablaVeleta(anVeleta);

var eEscalado= fit(anE/*valor*/,-511/*inicio*/,512/*fin*/, -135/*escala salida inicio*/,135/*escala salida fin*/);


var ret= '{"e":"'+eEscalado+'",'+
		 '"c":"'+anC+'",'+
		 '"pwm":"'+anPwm+'"}';

return ret;


}
