const express = require('express');

const router = express.Router();

/* Get all groups */
router.get('/', async (req, res) => {
  /* begin a SELECT query */
  req.context.models.Group.findAll()
    /* Return the JSON array */
    .then((groups) => res.status(200).json(groups))
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific group */
router.get('/:groupId', async (req, res) => {
  /* Get the group from the private key in the URL parameter */
  req.context.models.Group.findByPk(req.params.groupId)
    .then((group) => {
      if (group) {
        /* Return the information */
        return res.status(200).json(group);
      }
      else {
        /* Return a 404 error, not found */
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Create a group */
router.post('/', async (req, res) => {
  req.context.models.Group.bulkCreate(req.body)
    /* Successful insert */
    .then((group) => res.status(201).json(group))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Edit a group */
router.put('/:groupId', async (req, res) => {
  /* Query the database for the group and update its values */
  req.context.models.Group.update(req.body, {
      where: { id: req.params.groupId }
    })
    .then((n) => {
      /* Check the number of affect rows */
      if (n > 0) {
        /* If a row was affected, return the new data for the whole group */
        req.context.models.Group.findByPk(req.params.groupId)
          .then((group) => {
            /* Return the new group */
            return res.status(200).json(group);
          })
          /* Handle errors, this should theoretically never be reached */
          .catch((err) => {
            console.error(err);
            return res.status(400).json({ message: 'Bad Request' });
          });
      }
      else {
        /* There were no groups with that id */
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Remove a group */
router.delete('/:groupId', async (req, res) => {
  /* Query the db and delete the group */
  req.context.models.Group.destroy({
      where: { id: req.params.groupId }
    })
    .then((n) => {
      /* Check the number of affected rows */
      if (n > 0) {
        /* There were rows affected and the group was deleted */
        return res.status(200).json({ message: 'OK' });
      }
      else {
        /* There was no group with that id, not found */
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    })
});

module.exports = router;
