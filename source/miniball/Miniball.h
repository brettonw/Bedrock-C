#include <cassert>
#include <algorithm>
#include <list>
#include <ctime>
#include <limits>

// Global Functions
template <typename ScalarType>
inline ScalarType square (ScalarType r) { return r * r; }

// point type - tuple of scalars
// coordinate iterator

// Functors
// functor to map a point iterator to the corresponding coordinate iterator;
// generic version for points whose coordinate containers have begin()
template < typename PointIterator_, typename CoordinateIterator_ >
struct CoordinateAccessor {
    typedef PointIterator_ PointIterator;
    typedef CoordinateIterator_ CoordinateIterator;
    inline  CoordinateIterator operator() (PointIterator it) const { return (*it).begin(); }
};

// partial specialization for points whose coordinate containers are arrays
template < typename PointIterator_, typename CoordinateIterator_ >
struct CoordinateAccessor<PointIterator_, CoordinateIterator_*> {
    typedef PointIterator_ PointIterator;
    typedef CoordinateIterator_* CoordinateIterator;
    inline  CoordinateIterator operator() (PointIterator it) const { return *it; }
};

// Class Declaration

template <typename CoordinateAccessor>
class Miniball {
    private:

        // The iterator type to go through the input points
        typedef typename CoordinateAccessor::PointIterator PointIterator;
        // The iterator type to go through the coordinates of a single point.
        typedef typename CoordinateAccessor::CoordinateIterator CoordinateIterator;
        // The coordinate type
        typedef typename std::iterator_traits<CoordinateIterator>::value_type ScalarType;
        // The iterator to go through the support points
        typedef typename std::list<PointIterator>::iterator SupportIterator;

        typedef ScalarType* Coordinate;

        // data members...
        const int dimension; // dimension
        PointIterator pointsBegin;
        PointIterator pointsEnd;
        CoordinateAccessor coordinateAccessor;
        const ScalarType nt0; // ScalarType(0)

        //...for the algorithms
        std::list<PointIterator> supportPoints;
        SupportIterator supportPointsEnd;
        int fsize;   // number of forced points
        int ssize;   // number of support points

        // ...for the ball updates
        Coordinate  currentCenter;
        ScalarType  currentSquaredRadius;
        Coordinate* c;
        ScalarType* squaredRadius;

        // helper arrays
        ScalarType* q0;
        ScalarType* z;
        ScalarType* f;
        Coordinate* v;
        Coordinate* a;

        // by how much do we allow points outside?
        ScalarType defaultTolerance;

        void mtf_mb (SupportIterator n);
        void mtf_move_to_front (SupportIterator j);
        void pivot_mb (PointIterator n);
        void pivot_move_to_front (PointIterator j);
        ScalarType excess (PointIterator pointIterator) const;
        void pop ();
        bool push (PointIterator pointIterator);
        ScalarType suboptimality () const;
        void create_arrays();
        void delete_arrays();

    public:
        // The iterator type to go through the support points
        typedef typename std::list<PointIterator>::const_iterator SupportPointIterator;

        // PRE:  [begin, end) is a nonempty range
        // POST: computes the smallest enclosing ball of the points in the range
        //       [begin, end); the functor a maps a point iterator to an iterator
        //       through the dimension coordinates of the point
        Miniball (int dimension_, PointIterator begin, PointIterator end, CoordinateAccessor coordinateAccessor_ = CoordinateAccessor()) :
                dimension (dimension_),
                pointsBegin (begin),
                pointsEnd (end),
                coordinateAccessor (coordinateAccessor_),
                nt0 (ScalarType(0)),
                supportPoints(),
                supportPointsEnd (supportPoints.begin()),
                fsize(0),
                ssize(0),
                currentCenter (NULL),
                currentSquaredRadius (ScalarType(-1)),
                c (NULL),
                squaredRadius (NULL),
                q0 (NULL),
                z (NULL),
                f (NULL),
                v (NULL),
                a (NULL),
                defaultTolerance (ScalarType(10) * std::numeric_limits<ScalarType>::epsilon())
        {
            assert (pointsBegin != pointsEnd);
            create_arrays();

            // set initial center
            for (int j=0; j<dimension; ++j) c[0][j] = nt0;
            currentCenter = c[0];

            // compute miniball
            pivot_mb (pointsEnd);
        }

        // POST: returns a pointer to the first element of an array that holds
        //       the dimension coordinates of the center of the computed ball
        const ScalarType* getCenter () const {
            return currentCenter;
        }

        // POST: returns the squared radius of the computed ball
        ScalarType getSquaredRadius () const {
            return currentSquaredRadius;
        }

        // POST: returns the number of support points of the computed ball;
        //       the support points form a minimal set with the same smallest
        //       enclosing ball as the input set; in particular, the support
        //       points are on the boundary of the computed ball, and their
        //       number is at most dimension+1
        int getSupportPointCount () const {
            assert (ssize < dimension+2);
            return ssize;
        }

        // POST: returns an iterator to the first support point
        SupportPointIterator getSupportPointsBegin () const {
            return supportPoints.begin();
        }

        // POST: returns a past-the-end iterator for the range of support points
        SupportPointIterator getSupportPointsEnd () const {
            return supportPointsEnd;
        }

        // POST: returns the maximum excess of any input point w.r.t. the computed
        //       ball, divided by the squared radius of the computed ball. The
        //       excess of a point is the difference between its squared distance
        //       from the center and the squared radius; Ideally, the return value
        //       is 0. subopt is set to the absolute value of the most negative
        //       coefficient in the affine combination of the support points that
        //       yields the center. Ideally, this is a convex combination, and there
        //       is no negative coefficient in which case subopt is set to 0.
        ScalarType relative_error (ScalarType& subopt) const;

        // POST: return true if the relative error is at most tol, and the
        //       suboptimality is 0; the default tolerance is 10 times the
        //       coordinate type's machine epsilon
        bool is_valid () const {
            ScalarType suboptimality;
            return ((relative_error (suboptimality) <= defaultTolerance) and (suboptimality == 0));
        }

        // POST: deletes dynamically allocated arrays
        ~Miniball(){
            delete_arrays();
        }
};

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::create_arrays()
{
c = new ScalarType*[dimension+1];
v = new ScalarType*[dimension+1];
a = new ScalarType*[dimension+1];
for (int i=0; i<dimension+1; ++i) {
  c[i] = new ScalarType[dimension];
  v[i] = new ScalarType[dimension];
  a[i] = new ScalarType[dimension];
}
squaredRadius = new ScalarType[dimension+1];
q0 = new ScalarType[dimension];
z = new ScalarType[dimension+1];
f = new ScalarType[dimension+1];
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::delete_arrays()
{
delete[] f;
delete[] z;
delete[] q0;
delete[] squaredRadius;
for (int i=0; i<dimension+1; ++i) {
  delete[] a[i];
  delete[] v[i];
  delete[] c[i];
}
delete[] a;
delete[] v;
delete[] c;
}

template <typename CoordinateAccessor>
typename Miniball<CoordinateAccessor>::ScalarType
Miniball<CoordinateAccessor>::relative_error (ScalarType& subopt) const
{
ScalarType e, max_e = nt0;
// compute maximum absolute excess of support points
for (SupportPointIterator it = getSupportPointsBegin(); it != getSupportPointsEnd(); ++it) {
    e = excess (*it);
    if (e < nt0) e = -e;
    if (e > max_e) {
        max_e = e;
    }
}
// compute maximum excess of any point
for (PointIterator i = pointsBegin; i != pointsEnd; ++i)
  if ((e = excess (i)) > max_e)
max_e = e;

subopt = suboptimality();
assert (currentSquaredRadius > nt0 or max_e == nt0);
return (currentSquaredRadius == nt0 ? nt0 : max_e / currentSquaredRadius);
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::mtf_mb (SupportIterator n)
{
// Algorithm 1: mtf_mb (L_{n-1}, B), where L_{n-1} = [L.begin, n)
// B: the set of forced points, defining the current ball
// S: the superset of support points computed by the algorithm
// --------------------------------------------------------------
// from B. Gaertner, Fast and Robust Smallest Enclosing Balls, ESA 1999,
// http://www.inf.ethz.ch/personal/gaertner/texts/own_work/esa99_final.pdf

//   PRE: B = S
assert (fsize == ssize);

supportPointsEnd = supportPoints.begin();
if ((fsize) == dimension+1) return;

// incremental construction
for (SupportIterator i = supportPoints.begin(); i != n;)
  {
// INV: (supportPointsEnd - supportPoints.begin() == |S|-|B|)
assert (std::distance (supportPoints.begin(), supportPointsEnd) == ssize - fsize);

SupportIterator j = i++;
if (excess(*j) > nt0)
  if (push(*j)) {          // B := B + p_i
    mtf_mb (j);            // mtf_mb (L_{i-1}, B + p_i)
    pop();                 // B := B - p_i
    mtf_move_to_front(j);
  }
  }
// POST: the range [L.begin(), supportPointsEnd) stores the set S\B
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::mtf_move_to_front (SupportIterator j)
{
if (supportPointsEnd == j)
  supportPointsEnd++;
supportPoints.splice (supportPoints.begin(), supportPoints, j);
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::pivot_mb (PointIterator n)
{
// Algorithm 2: pivot_mb (L_{n-1}), where L_{n-1} = [L.begin, n)
// --------------------------------------------------------------
// from B. Gaertner, Fast and Robust Smallest Enclosing Balls, ESA 1999,
// http://www.inf.ethz.ch/personal/gaertner/texts/own_work/esa99_final.pdf
ScalarType          old_sqr_r;
const ScalarType*   c;
PointIterator         pivot, k;
ScalarType          e, max_e, squaredRadius;
CoordinateIterator p;
unsigned int loops_without_progress = 0;
do {
  old_sqr_r = currentSquaredRadius;
  squaredRadius = currentSquaredRadius;

  pivot = pointsBegin;
  max_e = nt0;
  for (k = pointsBegin; k != n; ++k) {
p = coordinateAccessor(k);
e = -squaredRadius;
c = currentCenter;
for (int j=0; j<dimension; ++j)
  e += square<ScalarType>(*p++-*c++);
if (e > max_e) {
  max_e = e;
  pivot = k;
}
  }

  if ((squaredRadius < 0) or (max_e > (squaredRadius * defaultTolerance))) {
// check if the pivot is already contained in the support set
if (std::find(supportPoints.begin(), supportPointsEnd, pivot) == supportPointsEnd) {
  assert (fsize == 0);
  if (push (pivot)) {
    mtf_mb(supportPointsEnd);
    pop();
    pivot_move_to_front(pivot);
  }
}
  }
  if (old_sqr_r < currentSquaredRadius)
loops_without_progress = 0;
  else
++loops_without_progress;
} while (loops_without_progress < 2);
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::pivot_move_to_front (PointIterator j)
{
supportPoints.push_front(j);
if (std::distance(supportPoints.begin(), supportPointsEnd) == (dimension + 2))
  supportPointsEnd--;
}

template <typename CoordinateAccessor>
inline typename Miniball<CoordinateAccessor>::ScalarType
Miniball<CoordinateAccessor>::excess (PointIterator pointIterator) const
{
CoordinateIterator p = coordinateAccessor(pointIterator);
ScalarType e = -currentSquaredRadius;
ScalarType* c = currentCenter;
for (int k=0; k<dimension; ++k){
  e += square<ScalarType>(*p++-*c++);
}
return e;
}

template <typename CoordinateAccessor>
void Miniball<CoordinateAccessor>::pop ()
{
--fsize;
}

template <typename CoordinateAccessor>
bool Miniball<CoordinateAccessor>::push (PointIterator pointIterator)
{
int i, j;
ScalarType eps = square<ScalarType>(std::numeric_limits<ScalarType>::epsilon());

CoordinateIterator cit = coordinateAccessor(pointIterator);
CoordinateIterator p = cit;

if (fsize==0) {
  for (i=0; i<dimension; ++i)
q0[i] = *p++;
  for (i=0; i<dimension; ++i)
c[0][i] = q0[i];
  squaredRadius[0] = nt0;
}
else {
  // set v_fsize to Q_fsize
  for (i=0; i<dimension; ++i)
//v[fsize][i] = p[i]-q0[i];
v[fsize][i] = *p++-q0[i];

  // compute the a_{fsize,i}, i< fsize
  for (i=1; i<fsize; ++i) {
a[fsize][i] = nt0;
for (j=0; j<dimension; ++j)
  a[fsize][i] += v[i][j] * v[fsize][j];
a[fsize][i]*=(2/z[i]);
  }

  // update v_fsize to Q_fsize-\bar{Q}_fsize
  for (i=1; i<fsize; ++i) {
for (j=0; j<dimension; ++j)
  v[fsize][j] -= a[fsize][i]*v[i][j];
  }

  // compute z_fsize
  z[fsize]=nt0;
  for (j=0; j<dimension; ++j)
z[fsize] += square<ScalarType>(v[fsize][j]);
  z[fsize]*=2;

  // reject push if z_fsize too small
  if (z[fsize]<eps*currentSquaredRadius) {
return false;
  }

  // update c, squaredRadius
  p=cit;
  ScalarType e = -squaredRadius[fsize-1];
  for (i=0; i<dimension; ++i)
e += square<ScalarType>(*p++-c[fsize-1][i]);
  f[fsize]=e/z[fsize];

  for (i=0; i<dimension; ++i)
c[fsize][i] = c[fsize-1][i]+f[fsize]*v[fsize][i];
  squaredRadius[fsize] = squaredRadius[fsize-1] + e*f[fsize]/2;
}
currentCenter = c[fsize];
currentSquaredRadius = squaredRadius[fsize];
ssize = ++fsize;
return true;
}

template <typename CoordinateAccessor>
typename Miniball<CoordinateAccessor>::ScalarType
Miniball<CoordinateAccessor>::suboptimality () const
{
ScalarType* l = new ScalarType[dimension+1];
ScalarType min_l = nt0;
l[0] = ScalarType(1);
for (int i=ssize-1; i>0; --i) {
  l[i] = f[i];
  for (int k=ssize-1; k>i; --k)
l[i]-=a[k][i]*l[k];
  if (l[i] < min_l) min_l = l[i];
  l[0] -= l[i];
}
if (l[0] < min_l) min_l = l[0];
delete[] l;
if (min_l < nt0)
  return -min_l;
return nt0;
}
