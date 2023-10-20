import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { ApiService } from '../services/api.service';

@Component({
  selector: 'app-buoys',
  templateUrl: './buoys.component.html',
  styleUrls: ['./buoys.component.css']
})
export class BuoysComponent implements OnInit {

  constructor(private api: ApiService, private router: Router) { }

  buoys: any[];

  ngOnInit(): void {
    this.api.getBouy().subscribe(res => {
      this.buoys = res;
    });
  }

  navigateToData(id: number) {
    this.router.navigateByUrl(`/data?id=${id}`);
  }

}
