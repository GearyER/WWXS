import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ApiService {

  private readonly apiEnpoint = "http://localhost:8080";
  private readonly buoyApiEndpoint = this.apiEnpoint + "/buoy"
  private readonly dataApiEndpoint = this.apiEnpoint + "/data"
  private readonly groupApiEndpoint = this.apiEnpoint + "/group"
  private readonly userApiEndpoint = this.apiEnpoint + "/user"
  private readonly userLoginApiEndpoint = this.userApiEndpoint + "/login"


  public getBouy(id?: number): Observable<any> {
    if (id) {
      return this.http.get(this.buoyApiEndpoint + '/' + id);
    }
    else {
      return this.http.get(this.buoyApiEndpoint);
    }
  }

  public postBuoy(body: any) {
    return this.http.post(this.buoyApiEndpoint, body);
  }

  public putBuoy(id: number, body:any) {
    return this.http.put(this.buoyApiEndpoint + '/' + id, body);
  }

  public deleteBuoy(id: number) {
    return this.http.delete(this.buoyApiEndpoint + '/' + id);
  }



  public getData(id?: number): Observable<any> {
    if (id) {
      return this.http.get(this.dataApiEndpoint + '/' + id);
    }
    else {
      return this.http.get(this.dataApiEndpoint);
    }
  }

  public postData(body: any) {
    return this.http.post(this.dataApiEndpoint, body);
  }


  public getGroup(id?: number): Observable<any> {
    if (id) {
      return this.http.get(this.groupApiEndpoint + '/' + id);
    }
    else {
      return this.http.get(this.groupApiEndpoint);
    }
  }

  public postGroup(body: any) {
    return this.http.post(this.groupApiEndpoint, body);
  }

  public putGroup(id: number, body:any) {
    return this.http.put(this.groupApiEndpoint + '/' + id, body);
  }

  public deleteGroup(id: number) {
    return this.http.delete(this.groupApiEndpoint + '/' + id);
  }


  public getUser(id?: number): Observable<any> {
    if (id) {
      return this.http.get(this.userApiEndpoint + '/' + id);
    }
    else {
      return this.http.get(this.userApiEndpoint);
    }
  }
  //get users by group id
  public getUsersByGroup(id: number): Observable<any> {
    return this.http.get(this.userApiEndpoint + '/group/' + id);
  }

  public postUser(body: any) {
    return this.http.post(this.userApiEndpoint, body);
  }

  public postUserLogin(body: any) {
    return this.http.post(this.userLoginApiEndpoint, body);
  }

  constructor(private http: HttpClient) { }
}
