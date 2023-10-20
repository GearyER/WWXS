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
      groupId: 1
    },
    {
      username: 'erik',
      // md5 hash for 'admin'
      password: '21232f297a57a5a743894a0e4a801fc3',
      role: 'dev',
      groupId: 1
    },
    {
      username: 'john',
      // md5 hash for 'password'
      password: '5f4dcc3b5aa765d61d8327deb882cf99',
      role: 'user',
      groupId: 2
    }
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
      name: 'proto',
      mac: 'FF-FF-FF-FF-FF-FF',
      groupId: 1,
      locationId: 1
    },
    {
      name: 'salish',
      mac: '12:34:56:78:9A:BC',
      groupId: 2,
      locationId: 1
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
  ]
};

module.exports = testData;
