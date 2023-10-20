import { Component, OnInit, Input, ComponentFactoryResolver } from '@angular/core';
import { ApiService } from '../services/api.service';
import { ActivatedRoute } from '@angular/router';
import { IDropdownSettings } from 'ng-multiselect-dropdown';
import { element } from 'protractor';
//import { Console } from 'console';

@Component({
  selector: 'line-graph',
  templateUrl: './line-graph.component.html',
  styleUrls: ['./line-graph.component.css']
})
export class LineGraphComponent implements OnInit{

  //fields for selected buoys drop down
  dropdownList;
  selected_items = [];
  dropdownSettings = {
    singleSelection: false,
    idField: 'item_id',
    textField: 'item_text',
    selectAllText: 'Select All',
    unSelectAllText: 'UnSelect All',
    itemsShowLimit: 2,
    allowSearchFilter: true
  };

  //array that will hold the two different arrays that our graph with display
  tempdata;
  //an integer used to switch between the two different arrays above
  flip = 0;

  /* chart options for drop down, add charts to give user more options */
  charts = ['Line Chart','Scatter Chart'];
  chartMap = new Map([
    ['Line Chart', 'LineChart'],
    ['Scatter Chart', 'ScatterChart']
  ]);
  type = this.chartMap.get('Line Chart'); // default chart shown


  /* data type options for drop down, should probably rename these i.e. 'surf temperature' rather than 'surfTemp' */
  dataTypes = ['Surface Temperature', 'Shallow Salinity', 'Surface Insolation', 'Depth Turbidity', 'Shallow Temperature', 'Depth Temperature']; 
  dataMap = new Map([
    ['Surface Temperature', 'surfTemp'],
    ['Shallow Salinity', 'shallowSalinity'],
    ['Surface Insolation', 'surfInsolation'],
    ['Depth Turbidity', 'depthTurbidity'],
    ['Shallow Temperature', 'shallowTemp'],
    ['Depth Temperature', 'depthTemp']
  ]);
  dataType = 'Surface Temperature'; //this.dataMap.get('Surface Temperature'); // default graph shown
  
  //var to hold data passed in ngOnInit(), maybe shouldn't hold all of it?
  buoy_data: any[];
  title: string; // title of the chart
  data;
  column_names: any[];
  width: number;
  height: number;

  /* options for Google Chart */
  options = {   
    hAxis: {
       title: 'Time'
    },
    vAxis:{
       title: 'Surface Temperature'
    },
  };

  queryParam: number;

  /* constructor */
  constructor(private api: ApiService, private route: ActivatedRoute){}

  ngOnInit(): void {

    this.route.queryParams.subscribe(params => {
      this.queryParam = params['id'];
    });

    /* get data from database */
    this.api.getData().subscribe(data => {

      this.buoy_data = Object.assign(Object.create(data), data); // get deep copy of data

      //populate buoy drop down
      var len = data.length;
      var count = 1;
      //var templist = [];
      this.dropdownList = new Array();
      for(var i = 0; i < len; i++){
        //we only need one instance of each name... are buoy names going to be unique at release?
        if(!this.locationIdExists(data[i].locationId, this.dropdownList))
          this.dropdownList.push({item_id: data[i].locationId, item_text: data[i].location.name});
      }

      this.width = 550; // for Google Chart purposes
      this.height = 400;
      this.column_names = ["Time"];
      this.plotData();
    });
  }

  /* function to plot the data */
  plotData(): void {
    // i made this in half an hour. it should definitely not be done this way.
    // basically: creating a 2D array with a number of rows equal to the number
    // of rows in the data table, and a number of columns equal to the number
    // of selected locations plus 1. the first column is always the timestamp.
    // the next columns represent the different locations.
    // the end result we build is something like this:
    //  Time        Loc1        Loc2
    //  01/01/22    17.3        null
    //  01/02/22    15.5        null
    //  01/01/22    null        12.4
    //  01/02/22    null        10.0
    // of note: each row will have a timestamp, and then every value afterwards
    // will be null except one. this is probably not how we should do this.
    // i'm sorry to whoever fixes this.
    
    this.tempdata = [
        Array.from({length: this.buoy_data.length}, () => Array.from({length: this.column_names.length}, () => null)),
        Array.from({length: this.buoy_data.length}, () => Array.from({length: this.column_names.length}, () => null))
    ];

    //looping through buoyData array
    for (let buoy_index = 0; buoy_index < this.buoy_data.length; buoy_index++) {
      this.tempdata[this.flip][buoy_index][0] = new Date(this.buoy_data[buoy_index].timestamp);

      var buoy_column = this.getColumnIndex(buoy_index);
      if(buoy_column == -1) // if the user does not select a buoy, it shouldn't fall in this for loop at all
      {
        continue;
      }

      //probably a more efficient way to do this outside of the for loop
      switch(this.dataMap.get(this.dataType)){

        /* Surface Temperature */
        case 'surfTemp': 
          this.title = "Surface Temperature Data"; // update title of graph   
          this.options.vAxis.title = "Surface Temp in Fahrenheit"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].surfTemp;
          break;

        /* Shallow Temperature */
        case 'shallowTemp': 
          this.title = "Surface Temperature Data"; // update title of graph   
          this.options.vAxis.title = "Surface Temp in Fahrenheit"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].shallowTemp;
          break;

        /* Depth Temperature */
        case 'depthTemp': 
          this.title = "Surface Temperature Data"; // update title of graph   
          this.options.vAxis.title = "Surface Temp in Fahrenheit"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].depthTemp;
          break;

        /* Surface Insolation */
        case 'surfInsolation': 
          this.title = "Surface Insolation Data"; // update title of graph  
          this.options.vAxis.title = "Surface Insolation in what units?"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].surfInsolation;
          break;

        /* Shallow Salinity */
        case 'shallowSalinity': 
          this.title = "Shallow Salinity Data"; // update title of graph
          this.options.vAxis.title = "Shallow Salinity in what units?"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].shallowSalinity;
          break;

        /* Surface Salinity */
        case 'surfSalinity':
          this.title = "Surface Salinity Data"; // update title of graph
          this.options.vAxis.title = "Surface Salinity in what units?"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].surfSalinity;
          break;

        /* Depth Turbidity */
        case 'depthTurbidity':
          this.title = "Depth Turbidity Data"; // update title of graph   
          this.options.vAxis.title = "Depth Turbidity in what units?"; // update y-axis label
          this.tempdata[this.flip][buoy_index][buoy_column] = this.buoy_data[buoy_index].depthTurbidity;
          break;
      }
    }

    //for some reason the graph won't update unless we assign a completely different array
    //so we keep flipping between the two arrays in tempdata and assign data to the newest one
    this.data = this.tempdata[this.flip];

    /* flip which array we'll be editing on the next call to plotdata() */
    if(this.flip)
      this.flip = 0;
    else
      this.flip = 1;
  }

  locationIdExists(id: number, dropdown: {item_id: number, item_text: string}[]): boolean {
    var len = dropdown.length;
    for(var i = 0; i < len; i++) {
      if(id === dropdown[i].item_id)
        return true;
    }
    return false;
  }

  //get index of a buoy in column_names given an index of a datapoint in buoy_data
  getColumnIndex(index: number): number {

    var len = this.column_names.length;
    //search each index of our column names to find the one that matches our buoy name
    // we start at 1 because index 0 is the time
    for(var i = 1; i < len; i++){
      if(this.column_names[i] === this.buoy_data[index].location.name)
        return i;
    }

    //didn't exist
    return -1;
  }

  //adjusts columns of our google chart
  //these columns make up the selected buoys
  adjustColumnsByItems(items: any[]): void{
    this.column_names = ["Time"];

    items.forEach(element => {
      //add a column for each buoy name
      this.column_names.push(element.item_text);
    });
  }

  //probably a better way to do this than have two functions
  adjustColumnsBySelected(): void{
    this.column_names = ["Time"];

    this.selected_items.forEach(element => {
      this.column_names.push(element.item_text);
    });
  }

  /* drop down chart type event handler */
  onChartSelect(event){
    this.type = this.chartMap.get(event.target.value);
  }

  /* Function to handle drop-down data-type event handler */
  onDataTypeSelect(event){
    this.dataType = this.dataMap.get(event.target.value);
    console.log("dataType from onDataTypeSelect = " + this.dataType);
    this.dataType = event.target.value;
    this.plotData();
  }

  //event handler for single select of buoy drop down
  onBuoySelect(item: any) {
    this.adjustColumnsBySelected();
    this.plotData();
  }
  //event handler for single deselect of buoy drop down
  onBuoyDeSelect(item: any) {
    this.adjustColumnsBySelected();
    this.plotData();
  }
  //event handler for select all of buoy drop down
  onBuoySelectAll(items: any) {
    this.adjustColumnsByItems(items);
    this.plotData();
  }
  //event handler for DeSelect all of buoy drop down
  onBuoyDeSelectAll(items: any) {
    this.adjustColumnsByItems(items);
    this.plotData();
  }


  printBuoyData() {

    console.log("in printBuoyData() function");
    console.log("this.buoy_data.length = " + this.buoy_data.length);

    for(let buoy_index = 0; buoy_index < this.buoy_data.length; buoy_index++)
    {
      console.log("this.buoyData["+buoy_index+"].surfTemp = " + this.buoy_data[buoy_index].surfTemp);
      console.log("this.buoyData["+buoy_index+"].shallowTemp = " + this.buoy_data[buoy_index].shallowTemp);
      console.log("this.buoyData["+buoy_index+"].depthTemp = " + this.buoy_data[buoy_index].depthTemp);
      console.log("this.buoyData["+buoy_index+"].surfInsolation = " + this.buoy_data[buoy_index].surfInsolation);
      console.log("this.buoyData["+buoy_index+"].shallowSalinity = " + this.buoy_data[buoy_index].shallowSalinity);
      console.log("this.buoyData["+buoy_index+"].depthTurbidity = " + this.buoy_data[buoy_index].depthTurbidity);
    }
  }
}
