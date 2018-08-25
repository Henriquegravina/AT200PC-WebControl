//t = setTimeout(function(){ all_values()}, 500);

function all_values(str = "REQ_ALLUPDATE"){
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //document.getElementById("demo").innerHTML = this.responseText;
            var obj=this.responseText;
            obj1 = JSON.parse(obj);
            //var obj1 = JSON.parse("{ \"swr_threshold\" : \"1:1\", \"hilo_z_string\" : \"1\", \"capval_string\" : \"123\", \"indval_string\" : \"120\" , \"instandby_string\" : \"standby\", \"at_status_string\" : \"off\", \"powerfwd_string\" : \"125\",\"powerrev_string\" : \"45.0\", \"swr_string\" : \"10\", \"live_update_string\" : \"on\", \"tx_frequency_string\" : \"135\"}");
            
            if(obj1.swr_threshold) document.getElementById("swr_threshold").innerHTML = obj1.swr_threshold;
            if(obj1.hilo_z_string)document.getElementById("hilo_z_string").innerHTML = obj1.hilo_z_string;
            
            if(obj1.capval_string)document.getElementById("capval_string").innerHTML = obj1.capval_string;
            if(obj1.powerfwd_string)document.getElementById("CAP_bar").style.width = ( obj1.capval_string) + "%";
            
            if(obj1.indval_string)document.getElementById("indval_string").innerHTML = obj1.indval_string;
            if(obj1.powerfwd_string)document.getElementById("IND_bar").style.width = ( obj1.indval_string) + "%";
            
            if(obj1.instandby_string)document.getElementById("instandby_string").innerHTML = obj1.instandby_string;
            if(obj1.at_status_string)document.getElementById("at_status_string").innerHTML = obj1.at_status_string;
            
            if(obj1.powerfwd_string)document.getElementById("powerfwd_string").innerHTML = obj1.powerfwd_string;
            if(obj1.powerfwd_string)document.getElementById("PWR_bar").style.width = ( obj1.powerfwd_string) + "%";
            
            if(obj1.powerrev_string)document.getElementById("powerrev_string").innerHTML = obj1.powerrev_string;
            if(obj1.powerrev_string)document.getElementById("SWR_bar").style.width = ( obj1.powerrev_string) + "%";
            
            
            
            if(obj1.swr_string)document.getElementById("swr_string").innerHTML = obj1.swr_string;
            if(obj1.swr_string)document.getElementById("live_update_string").innerHTML = obj1.live_update_string;
            if(obj1.tx_frequency_string)document.getElementById("tx_frequency_string").innerHTML = obj1.tx_frequency_string;
            if(str ==  "REQ_ALLUPDATE" ) { t = setTimeout(function(){ all_values()}, 1700);}
            }
    };
    xhttp.open("GET", "/"+str, true);
    xhttp.send();    
}

function showHintAnt(str) {
    var xhttp;
    if (str.length == 0) { 
        document.getElementById("txtHint_antenna").innerHTML = "";
        return;
    }
    xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("txtHint_antenna").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/"+str, true);
    xhttp.send();   
}

function showHintSwrT(str) {
    var xhttp;
    if (str.length == 0) { 
        document.getElementById("txtHint_swr").innerHTML = "";
        return;
    }
    xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("txtHint_swr").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/"+str, true);
    xhttp.send();   
}