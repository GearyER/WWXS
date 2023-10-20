import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { ApiService } from '../services/api.service';
import { AuthService } from '../services/auth.service';

@Component({
  selector: 'app-group',
  templateUrl: './group.component.html',
  styleUrls: ['./group.component.css']
})
export class GroupComponent implements OnInit {


  constructor(private api: ApiService, private router: Router,private auth: AuthService) { }

  buoys: any[];
  user: any;
  users: any[];

  ngOnInit(): void {
    /*this.api.getBouy().subscribe(res => {
      this.buoys = res;
    });
    this.api.getUser(this.auth.getUserData().id).subscribe(user => {
      this.user = user;
    });
    //get users by user group id
    this.api.getUsersByGroup(this.auth.getUserData().group_id).subscribe(users => {
      this.users = users;
    });*/
  }

  navigateToData(id: number) {
    this.router.navigateByUrl(`/data?id=${id}`);
  }
  //create toggleTables function to toggle between buoy and member divs
  toggleTables(){
    var buoy = document.getElementById("buoy");
    var member = document.getElementById("member");
    if(buoy.style.display === "none"){
      buoy.style.display = "block";
      member.style.display = "none";
    }
    else{
      buoy.style.display = "none";
      member.style.display = "block";
    }
  }


}
