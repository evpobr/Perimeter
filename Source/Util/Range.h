// TODO: change encoding to utf-8

#ifndef __RANGE_H_INCLUDED__
#define __RANGE_H_INCLUDED__

/// ���������� ��������� ��������� (�������).
template<typename ScalarType = float>
class Range
{
public:
  typedef Range<ScalarType> RangeType;

  Range (ScalarType _min, ScalarType _max) :
      min_ (_min),
      max_ (_max)
  {}

  inline ScalarType minimum () const 
  {
      return min_;
  }
  inline void minimum (ScalarType _min) 
  {
      min_ = _min;
  }
  inline ScalarType maximum () const 
  {
      return max_;
  }
  inline void maximum (ScalarType _max) 
  {
      max_ = _max;
  }

  inline ScalarType length ()
  {
      return (maximum () - minimum ());
  }

  /// ��������� �� �������� (��� - � ������ ����� minimum > maximum);
  inline bool is_valid () const
  {
      return (minimum () <= maximum ());
  }

  /// �������� �� ������� (�������� ��������) ����� \c _value.
  inline bool include (ScalarType _value) const
  {
      return (minimum () <= _value) && (maximum () >= _value);
  }
  /// �������� �� �������� � ���� \c _range.
  inline bool include (const RangeType& _range) const
  {
      return (minimum () <= _range.minimum ()) && (maximum () >= _range.maximum ());
  }

  /// ���������� ����������� ��������� *this � \c _range.
  inline RangeType intersection (const RangeType& _range)
  {
      ScalarType begin;
      ScalarType end;
      if (include (_range.minimum ()))
          begin = _range.minimum ();
      else
          begin = minimum ();

      if (include (_range.maximum ()))
          end = _range.maximum ();
      else
          end = maximum ();
      return RangeType (begin, end);
  }

  /// ���������� \c _value � �������� ��������� [minimum, maximum].
  inline ScalarType clip (ScalarType& _value)
  {
      if (include (_value))
          return _value;
      else
      {
          if (_value < minimum ())
              return minimum ();
          else
              return maximum ();
      }
  }

  template<class Archive>
  void serialize (Archive& ar)
  {
      ar & TRANSLATE_OBJECT (min_, "�������");
      ar & TRANSLATE_OBJECT (max_, "��������");
  }

private:
  ScalarType min_;
  ScalarType max_;
};

typedef Range<float> Rangef;

#endif
