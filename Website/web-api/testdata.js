console.log
const testData = { // TODO: Associate records
  users: [
    // DO NOT DO THIS WITH PASSWORDS IN PRODUCTION.
    // passwords should be salted and hashed, currently just hashed.
    // here, we're using the hashes of the example user passwords
    // so that, when the UI or app hashes these passwords for login,
    // they will match and the login will be successful.
    // when actually logging in, don't use the hashes shown below,
    // use the passwords provided in the comments.
    {
      username: 'kris',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      role: 'dev',
      groupId: 1,
    },
    {
      username: 'erik@example.com',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      role: 'dev',
      groupId: 1,
    },
    {
      username: 'john',
      // md5 hash for 'password'
      password: '5f4dcc3b5aa765d61d8327deb882cf99',
      role: 'user',
      groupId: 2
    },
    {
      username: 'admin@admin.com',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      role: 'dev',
      groupId: 1,
    },
  ],
  groups: [
    {
      name: 'default'
    },
    {
      name: 'group1'
    }
  ],
  locations: [
      {
          name: 'Boulevard Park 1',
          latitude:   48.73343624214479,
          longitude: -122.50343444257562
      },
      {
          name: 'Old Main',
          latitude: 48.73751,
          longitude: -122.48429
      }
  ],
  buoys: [
    {
      name: 'test',
      mac: 'FF-FF-FF-FF-FF-FF',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      groupId: 1,
      locationId: 1,
      userId: 2,
    },
    {
      name: 'salish',
      mac: '12:34:56:78:9A:BC',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      groupId: 2,
      locationId: 1,
      userId: 2,
    },
    {
      name: 'Proto',
      mac: '34:98:7A:4C:D4:46',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      groupId: 2,
      locationId: 1,
      userId: 4,
    },
    {
      name: 'Proto2',
      mac: '08:3A:F2:5B:D8:46',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      groupId: 2,
      locationId: 1,
      userId: 4,
    }
  ],
  data: [
    {
      timestamp: new Date(),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()+(1*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()+(2*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()+(3*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()+(4*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()+(5*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 1
    },
{
      timestamp: new Date(new Date().getTime()+(2*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()+(3*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()+(4*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()+(5*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()+(6*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()+(8*24*60*60*1000)),
      surfTemp: Math.random() * 100,
      surfInsolation: Math.random(),
      shallowSalinity: Math.random(),
      shallowTemp: Math.random(),
      depthTemp: Math.random(),
      depthTurbidity: Math.random(),
      locationId: 2
    }
  ],
  history: [
    {
      timestamp: new Date(new Date().getTime()),
      buoyId: 1,
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()) + 1,
      buoyId: 2,
      locationId: 1
    },
    {
      timestamp: new Date(new Date().getTime()) + 2,
      buoyId: 1,
      locationId: 2
    },
    {
      timestamp: new Date(new Date().getTime()) + 3,
      buoyId: 2,
      locationId: 2
    },
  ],
  managers: [
    {
      name: 'default',
      buoyId: 1,
      userId: 1
    },
    {
      name: 'manager1',
      buoyId: 2,
      userId: 1
    }
  ],
  authorized: [
    {
      name: 'default',
      buoyId: 2,
      userId: 1
    },
    {
      name: 'manager1',
      buoyId: 3,
      userId: 3
    }
  ]
};

module.exports = testData;
