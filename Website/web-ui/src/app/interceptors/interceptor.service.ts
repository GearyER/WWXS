import { Injectable } from '@angular/core';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor } from '@angular/common/http';
import { AuthService } from '../services/auth.service';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class InterceptorService implements HttpInterceptor {

  constructor(private auth: AuthService) { }

  intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    //if is authorized, add token to request and set headers
    //if (this.auth.isAuth()) {
      //request = request.clone({
        //setHeaders: {
          //Authorization: `bearer ${this.auth.getLocalToken()}`,
          //'Content-Type': 'application/json'
        //}
      //});
    //}
    return next.handle(request);
  }
}
