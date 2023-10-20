import { Component, OnInit } from '@angular/core';
import { ApiService } from '../services/api.service';
import { AuthService } from '../services/auth.service';

@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css']
})
export class ProfileComponent implements OnInit {

  constructor(private api: ApiService, public auth: AuthService) { }

  user: any;

  ngOnInit(): void {
    this.api.getUser(this.auth.getUserData().uid).subscribe(user => {
      this.user = user;
    });
  }

}
