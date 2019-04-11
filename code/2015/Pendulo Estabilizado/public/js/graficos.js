    var protocol = new Object;

    protocol["kp"] = {
      pic2web: function(value) {
        document.getElementById("kpvalue").innerHTML = value;
        document.getElementById("kp").value = value;
        document.getElementById("kpvalue").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("kpvalue").style.color = "#EEEE11";
        return "p" + document.getElementById("kp").value + "\r";
      }
    };
    protocol["ki"] = {
      pic2web: function(value) {
        document.getElementById("kivalue").innerHTML = value;
        document.getElementById("ki").value = value;
        document.getElementById("kivalue").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("kivalue").style.color = "#EEEE11";
        return "i" + document.getElementById("ki").value + "\r";
      }
    };
    protocol["kd"] = {
      pic2web: function(value) {
        document.getElementById("kdvalue").innerHTML = value;
        document.getElementById("kd").value = value;
        document.getElementById("kdvalue").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("kdvalue").style.color = "#EEEE11";
        return "d" + document.getElementById("kd").value + "\r";
      }
    };
    protocol["kp2"] = {
      pic2web: function(value) {
        document.getElementById("kp2value").innerHTML = value;
        document.getElementById("kp2").value = value;
        document.getElementById("kp2value").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("kp2value").style.color = "#EEEE11";
        return "q" + document.getElementById("kp2").value + "\r";
      }
    };
    protocol["ki2"] = {
      pic2web: function(value) {
        document.getElementById("ki2value").innerHTML = value;
        document.getElementById("ki2").value = value;
        document.getElementById("ki2value").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("ki2value").style.color = "#EEEE11";
        return "w" + document.getElementById("ki2").value + "\r";
      }
    };
    protocol["kd2"] = {
      pic2web: function(value) {
        document.getElementById("kd2value").innerHTML = value;
        document.getElementById("kd2").value = value;
        document.getElementById("kd2value").style.color = "#4CAF50";
      },
      web2pic: function() {
        document.getElementById("kd2value").style.color = "#EEEE11";
        return "e" + document.getElementById("kd2").value + "\r";
      }
    };
    protocol["cOn"] = {
      pic2web: function(value) {
        document.getElementById("cOncheck").checked = value;
        if(value){
            document.getElementById("cOn").style.background = "#4CAF50";
        }else{
            document.getElementById("cOn").style.background = "#d3d3d3";
        }

      },
      web2pic: function() {
        document.getElementById("cOn").style.background = "yellow";
        // console.log("Checked: " + document.getElementById("cOn").checked);
        var algo = "c" + ((document.getElementById("cOncheck").checked) ? '1' : '0') + "\r";
        // console.log("Comando: " + algo);
        return algo;
      }
    };
    protocol["ds0"] = {
      pic2web: function(value) {},
      web2pic: function() {
        document.getElementById("ds0").style.background = "yellow";
        document.getElementById("ds1").style.background = "#d3d3d3";
        document.getElementById("ds2").style.background = "#d3d3d3";
        return "m0\r";
      }
    };
    protocol["ds1"] = {
      pic2web: function(value) {},
      web2pic: function() {
        document.getElementById("ds0").style.background = "#d3d3d3";
        document.getElementById("ds1").style.background = "yellow";
        document.getElementById("ds2").style.background = "#d3d3d3";
        return "m1\r";
      }
    };
    protocol["ds2"] = {
      pic2web: function(value) {},
      web2pic: function() {
        document.getElementById("ds0").style.background = "#d3d3d3";
        document.getElementById("ds1").style.background = "#d3d3d3";
        document.getElementById("ds2").style.background = "yellow";
        return "m2\r";
      }
    };
    protocol["ds"] = {
      pic2web: function(value) {
        var id = "ds" + value;
        document.getElementById(id + "check").checked = true;
        document.getElementById("ds0").style.background = "#d3d3d3";
        document.getElementById("ds1").style.background = "#d3d3d3";
        document.getElementById("ds2").style.background = "#d3d3d3";
        document.getElementById(id).style.background = "#4CAF50";
      },
      web2pic: function() {
        return "m0\r";
      }
    };
    protocol["ps"] = {
      pic2web: function(value) {
        document.getElementById("ps").value = value;
        document.getElementById("ps").style.color = "#4CAF50";
      },
      web2pic: function() {
        return "n" + document.getElementById("ps").value + "\r";
      }
    };
    protocol["dm"] = {
      pic2web: function(value) {
        document.getElementById("dm").value = value;
        document.getElementById("dm").style.color = "#4CAF50";
      },
      web2pic: function() {
        return "3" + document.getElementById("dm").value + "\r";
      }
    };
    protocol["dl"] = {
      pic2web: function(value) {
        document.getElementById("dl").value = value;
        document.getElementById("dl").style.color = "#4CAF50";
      },
      web2pic: function() {
        return "1" + document.getElementById("dl").value + "\r";
      }
    };
    protocol["dt"] = {
      pic2web: function(value) {
        document.getElementById("dt").value = value;
        document.getElementById("dt").style.color = "#4CAF50";
      },
      web2pic: function() {
        return "2" + document.getElementById("dt").value + "\r";
      }
    };
    protocol["r"] = {
      pic2web: function(value) { /*console.log("recibe r con valor "+value)*/ ;
      },
      web2pic: function() {
        return "r56\r";
      }
    };
    protocol["z"] = {
      pic2web: function(value) { /*console.log("recibe z con valor "+value)*/ ;
      },
      web2pic: function() {
        return "z\r";
      }
    };
    protocol["s"] = {
      pic2web: function(value) { /*console.log("recibe s con valor "+value)*/ ;
      },
      web2pic: function() {
        return "s" + document.getElementById("s").value + "\r";
      }
    };

    var socket;
    var presets = new Object;

    function verificarCampo(event, campo) {
      event.preventDefault();
      campo.style.color = "blue"
      if (event.keyCode === 13) {
        campo.style.color = "yellow"
        enviar(campo.id);
      }
      if (event.keyCode == 27) { // 27 is the ESC key
        alert("You pressed the Escape key!");
      }
    }


    function getGraficasStructure() {
      var graficas = {};
      graficas['errorYcontrol'] = {
        titulo: 'Error y Control',
        id: 'errorYcontrol',
        unidades: {
          e: 'º',
          c: ''
        },
        lineas: {
          e: {
            nombre: 'Error',
            unidad: 'º',
            lineWidth: 1.8,
            strokeStyle: '#006400',
            /*fillStyle: 'rgba(107,184,110,0.60)',*/
            grid: {
              fillStyle: '#eeffee'
            }
          },
          c: {
            nombre: 'Control',
            unidad: 'watts',
            lineWidth: 0.8,
            strokeStyle: '#640000',
            /*fillStyle: 'rgba(107,184,110,0.60)',*/
            grid: {
              fillStyle: '#eeffee'
            }
          }
        },
        chartConfig: {
          millisPerPixel: 20,
          horizontalLines: [{
            color: '#000000',
            lineWidth: 1,
            value: 0
          }, {
            color: '#880000',
            lineWidth: 1,
            value: 164
          }, {
            color: '#880000',
            lineWidth: 1,
            value: -164
          }],
          minValue: -500,
          maxValue: 500,
          grid: {
            fillStyle: '#ffffff',
            strokeStyle: '#c5c5c5',
            sharpLines: true,
            millisPerLine: 2000,
            verticalSections: 8,
            borderVisible: true
          },
          labels: {
            fillStyle: 'rgba(0,0,0,0.5)',
            precision: 1
          },
          timestampFormatter: SmoothieChart.timeFormatter
        }
      }
      graficas['error'] = {
        titulo: 'Error',
        id: 'error',
        unidades: {
          e: 'º'
        },
        lineas: {
          e: {
            nombre: 'Error',
            unidad: 'º',
            lineWidth: 1.8,
            strokeStyle: '#006400',
            /*fillStyle: 'rgba(107,184,110,0.60)',*/
            grid: {
              fillStyle: '#eeffee'
            }
          }
        },
        chartConfig: {
          millisPerPixel: 20,
          horizontalLines: [{
            color: '#000000',
            lineWidth: 1,
            value: 0
          }, {
            color: '#880000',
            lineWidth: 1,
            value: 164
          }, {
            color: '#880000',
            lineWidth: 1,
            value: -164
          }],
          minValue: -500,
          maxValue: 500,
          grid: {
            fillStyle: '#ffffff',
            strokeStyle: '#c5c5c5',
            sharpLines: true,
            millisPerLine: 2000,
            verticalSections: 8,
            borderVisible: true
          },
          labels: {
            fillStyle: 'rgba(0,0,0,0.5)',
            precision: 1
          },
          timestampFormatter: SmoothieChart.timeFormatter
        }
      }
      graficas['control'] = {
        titulo: 'Control',
        id: 'control',
        unidades: {
          c: ''
        },
        lineas: {
          c: {
            nombre: 'Control',
            unidad: 'watts',
            lineWidth: 1.8,
            strokeStyle: '#006400',
            /*fillStyle: 'rgba(107,184,110,0.60)',*/
            grid: {
              fillStyle: '#eeffee'
            }
          }
        },
        chartConfig: {
          millisPerPixel: 20,
          horizontalLines: [{
            color: '#000000',
            lineWidth: 1,
            value: 0
          }, {
            color: '#880000',
            lineWidth: 1,
            value: 164
          }, {
            color: '#880000',
            lineWidth: 1,
            value: -164
          }],
          minValue: -1500,
          maxValue: 1500,
          grid: {
            fillStyle: '#ffffff',
            strokeStyle: '#c5c5c5',
            sharpLines: true,
            millisPerLine: 2000,
            verticalSections: 8,
            borderVisible: true
          },
          labels: {
            fillStyle: 'rgba(0,0,0,0.5)',
            precision: 1
          },
          timestampFormatter: SmoothieChart.timeFormatter
        }
      }
      return graficas;
    }

    function createGrid(grafico) {
      var gridDiv = document.createElement('div');
      grafico.canvas = document.createElement('canvas');
      grafico.canvas.id = grafico.id;
      gridDiv.className += " grid";
      gridDiv.setAttribute("id", grafico.id + 'Div');
      gridDiv.onclick = function() {
        if (this.className.indexOf('Full') != -1) {
          this.className = 'grid';
        } else {
          this.className = 'gridFull';
        }
        resizeCanvas(grafico.canvas);
      };
      grafico.divText = document.createElement('div');
      grafico.divText.setAttribute("id", grafico.id + 'Title');
      grafico.divText.className += " gridText";
      grafico.divText.innerHTML = grafico.titulo;
      gridDiv.appendChild(grafico.divText)
      gridDiv.appendChild(grafico.canvas)
      var graficasDiv=document.getElementById('graficasDiv');

      graficasDiv.appendChild(gridDiv);
      window.addEventListener('resize', resizeCanvas(grafico.canvas), false);
      resizeCanvas(grafico.canvas);
      console.log(grafico.canvas);
    }

    function resizeCanvas(canvas) {
      canvas.style.width = '100%';
      canvas.style.height = '100%';
      canvas.width = canvas.offsetWidth;
      canvas.height = canvas.offsetHeight;
    }

    function isNumeric(n) {
      return !isNaN(parseFloat(n)) && isFinite(n);
    }



    function enviar(parameterKey) {
      //console.log(typeof protocol[parameterKey])
      var arg;
      if (protocol.hasOwnProperty(parameterKey)) {
        arg = protocol[parameterKey].web2pic();
        socket.emit('clientData', arg);
      }
      /* else if (parameterKey == "all") {
                    for (var key in protocol.keys()) {
                      arg = protocol[key].web2pic();
                      socket.emit('clientData', arg)
                    }*/
      //socket.emit('clientData', "r1\r")
      //}
      if (!(parameterKey.indexOf('r') > -1)) {
        setTimeout(function() {
          enviar('r');
        }, 50);
      }
    }

    function cargarPreset(presetKey) {
      var thisPreset = presets[presetKey];
      for (var campo in thisPreset) {
        document.getElementById(campo).value = thisPreset[campo];
      }
    }

    function borrarPreset() {
      delete presets[document.getElementById('PresetsSelect').value];
      socket.emit('guardarPresets', presets);
        socket.emit('pedirPresets');
    }

    function nuevoPreset() {
      document.getElementById('PresetsSelect').style.display = 'none';
      document.getElementById('nombrePreset').value = "";
      document.getElementById('descripcionPreset').value = "";
      document.getElementById('nombrePreset').style.display = 'inline';
      document.getElementById('btnNuevoPreset').style.display = 'none';
      document.getElementById('btnGuardarPreset').style.display = 'inline';
    }

    function pedirPresets() {
      socket.emit('pedirPresets');
    }

    function guardarPresets() {
      document.getElementById('PresetsSelect').style.display = 'inline';
      document.getElementById('nombrePreset').style.display = 'none';
      document.getElementById('btnNuevoPreset').style.display = 'inline';
      document.getElementById('btnGuardarPreset').style.display = 'none';
      var nombrePreset = document.getElementById('nombrePreset').value;
      presets[nombrePreset] = {
        "descripcionPreset": document.getElementById('descripcionPreset').value,
        "kp": document.getElementById('kp').value,
        "ki": document.getElementById('ki').value,
        "kd": document.getElementById('kd').value,
        "kp2": document.getElementById('kp2').value,
        "ki2": document.getElementById('ki2').value,
        "kd2": document.getElementById('kd2').value,
        "dm": document.getElementById('dm').value,
        "dl": document.getElementById('dl').value,
        "dt": document.getElementById('dt').value,
        "ps": document.getElementById('ps').value,
        "s": document.getElementById('s').value
      }

      socket.emit('guardarPresets', presets);
        socket.emit('pedirPresets');
    }

    function inicializarGraficos() {
      graficas = getGraficasStructure();
      var seriesKeys = [];
      for (var grafico in graficas) {
        if (graficas.hasOwnProperty(grafico)) {
          createGrid(graficas[grafico]);
          graficas[grafico].chart = new SmoothieChart(graficas[grafico].chartConfig);
          graficas[grafico].chart.streamTo(graficas[grafico].canvas, 0 /*ms*/ );
          for (var linea in graficas[grafico].lineas) {
            if (graficas[grafico].lineas.hasOwnProperty(linea)) {
              if (seriesKeys.indexOf(linea) === -1) {
                seriesKeys.push(linea);

                var divLiveVars = document.getElementById('liveVars');
                var divVar = document.createElement('div');
                divVar.innerHTML = graficas[grafico].lineas[linea].nombre + '  <span id="' + linea + 'Live"></span>  ' + graficas[grafico].lineas[linea].unidad;
                divLiveVars.appendChild(divVar);
                //document.getElementById('liveVars').appendChild(document.createElement('div').setAttribute("id", linea + 'Live'));
              }
            }
          }

          console.log(seriesKeys);
        }
      }

      var series = new Object;
      seriesKeys.forEach(function(key) {
        series[key] = new TimeSeries();
        var now = new Date().getTime();
        for (var t = now - 1000 * 20; t <= now; t += 1000) {
          series[key].append(t, series[key]);
        }
      });
      for (var grafico in graficas) {
        if (graficas.hasOwnProperty(grafico)) {
          for (var linea in graficas[grafico].lineas) {
            if (graficas[grafico].lineas.hasOwnProperty(linea)) {
              console.log(linea);
              graficas[grafico].chart.addTimeSeries(series[linea], graficas[grafico].lineas[linea]);
            }
          }
        }
      }

      //SOCKET IO
      socket = io({
        'connect_timeout': 1000,
        'reconnection': true,
        'reconnectionDelay': 1000,
        'reconnectionDelayMax': 3000,
        'reconnectionAttempts': 10,
        'forceNew': true
      }); //io declarada en socketio.js

      //console.log("config socket:"+socket);

      socket.on('connect', function() {
        console.log("connect")
      });
      socket.on('disconnect', function() {
        console.log("disconnect")
      });
      //
      // socket.on('listaPresets', function(lista) {
      //   var select = document.getElementById("PresetsSelect");
      //   for (var i = select.options.length - 1; i >= 0; i--) {
      //     select.remove(i);
      //   }
      //   lista.forEach(function(elemento) {
      //     var option = document.createElement("option");
      //     option.text = elemento;
      //     select.add(option);
      //   });
      // });
      socket.on('valoresPreset', function(valores) {
        presets = valores;
        console.log(presets);
        var select = document.getElementById("PresetsSelect");
        for (var i = select.options.length - 1; i >= 0; i--) {
          select.remove(i);
        }
        for (var elemento in presets) {
          var option = document.createElement("option");
          option.text = elemento;
          select.add(option);
        }
      });

      socket.on('newServerData', function(dataStr) { // Al recibir datos desde el servidor
        console.log("rx remote:" + dataStr);
        var data = JSON.parse(dataStr); // los guarda en data

        // Ejemplo de data:
        // {"t":0,"e":0,"c":0,"p":0,"r":1,"kp":1,"ki":0,"kd":0,"kp2":1,"ki2":0,"kd2":0,"cOn":1,"ds":1,"ps":700,"dm":5,"dl":600,"dt":5}     []: Solo se recibe a pedido de r=1
        /*
	            t   -> PIC -> tik           -> descripcion:
	            e   -> PIC -> error         -> descripcion:
	            c   -> PIC -> ctrl          -> descripcion:
	            p   -> PIC -> dutyPWM       -> descripcion:
                r   -> PIC -> retornoConfig -> descripcion:
                kp  -> PIC -> Kp            -> descripcion:
                ki  -> PIC -> Ki            -> descripcion:
                kd  -> PIC -> Kd            -> descripcion:
                kp2 -> PIC -> Kp2           -> descripcion:
                ki2 -> PIC -> Ki2           -> descripcion:
                kd2 -> PIC -> Kd2           -> descripcion:
                cOn -> PIC -> controlOn     -> descripcion:
                ds  -> PIC -> dirStep       -> descripcion:
                ps  -> PIC -> potenciaStep  -> descripcion:
                dm  -> PIC -> dutyPWMmin    -> descripcion:
                dl  -> PIC -> dutyPWMlimit  -> descripcion:
                dt  -> PIC -> dutyPWMlimitTimeout -> descripcion:
            */


        for (var elemento in data) {
          if ((data.hasOwnProperty(elemento)) && !(seriesKeys.indexOf(elemento) === -1)) {
            var valor = data[elemento];
            var valToShow = Math.round(valor * 10) / 10;
            document.getElementById(elemento + 'Live').innerHTML = (isNumeric(valToShow) ? valToShow : valor);
            series[elemento].append(new Date().getTime(), valor);
          }
        }

        if (typeof data['r'] != 'undefined') {
          for (var datakey in data) {
            if (protocol.hasOwnProperty(datakey)) {
              protocol[datakey].pic2web(data[datakey]);
            }
          }
        }
      });
    }
    inicializarGraficos();
