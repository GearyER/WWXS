import { ComponentFixture, TestBed } from '@angular/core/testing';

import { BuoysComponent } from './buoys.component';

describe('BuoysComponent', () => {
  let component: BuoysComponent;
  let fixture: ComponentFixture<BuoysComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ BuoysComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(BuoysComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
