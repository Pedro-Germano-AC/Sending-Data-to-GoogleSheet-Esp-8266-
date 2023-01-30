function doGet(e) { 
    Logger.log( JSON.stringify(e) );
    var result = 'Ok';
    if (e.parameter == 'undefined') {
        result = 'No Parameters';
    }
    else {
        var sheet_id = 'SHEET_ID'; // Spreadsheet ID
        var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
        var newRow = sheet.getLastRow() + 1; 
        var rowData = [];
        var Curr_Date = new Date();
        rowData[0] = Curr_Date; // Date in column A
        var Curr_Time = Utilities.formatDate(Curr_Date, "America/Fortaleza", 'HH:mm:ss');
        rowData[1] = Curr_Time; // Time in column B
        for (var param in e.parameter) {
            Logger.log('In for loop, param=' + param);
            var value = stripQuotes(e.parameter[param]);
            Logger.log(param + ':' + e.parameter[param]);
            switch (param) {
                //Adicione aqui as variáveis que você utilizará para fazer a requisição (Envio dos dados)
                //Caso tenha mais de um, basta adicionar um bloco de código, seguindo o mesmo padrão e somando 1 ao valor que
                //Está no "RowData[]"

                case 'distanceCm':
                    rowData[2] = value; 
                    result = 'OK'; 
                    break;

                case 'temperature':
                    rowData[3] = value; 
                    result = 'OK'; 
                break;
                
                case 'humidity':
                    rowData[4] = value; 
                    result += ', OK'; 
                break; 
                
                default:
                    result = "Parametro nao suportado!";
            }
        }
        Logger.log(JSON.stringify(rowData));
        var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
        newRange.setValues([rowData]);
    }
    return ContentService.createTextOutput(result);
    }
    function stripQuotes( value ) {
        return value.replace(/^["']|['"]$/g, "");
    }
    
    
    
    