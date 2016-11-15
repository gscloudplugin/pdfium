// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFAPI_PARSER_CPDF_INDIRECT_OBJECT_HOLDER_H_
#define CORE_FPDFAPI_PARSER_CPDF_INDIRECT_OBJECT_HOLDER_H_

#include <map>
#include <memory>

#include "core/fxcrt/fx_system.h"
#include "third_party/base/ptr_util.h"

class CPDF_Object;

class CPDF_IndirectObjectHolder {
 public:
  using const_iterator =
      std::map<uint32_t, std::unique_ptr<CPDF_Object>>::const_iterator;

  CPDF_IndirectObjectHolder();
  virtual ~CPDF_IndirectObjectHolder();

  CPDF_Object* GetIndirectObject(uint32_t objnum) const;
  CPDF_Object* GetOrParseIndirectObject(uint32_t objnum);
  void DeleteIndirectObject(uint32_t objnum);

  // Creates and adds a new object owned by the indirect object holder,
  // and returns an unowned pointer to it.
  template <typename T, typename... Args>
  T* NewIndirect(Args... args) {
    return static_cast<T*>(AddIndirectObject(pdfium::MakeUnique<T>(args...)));
  }

  // Takes ownership of |pObj|, returns unowned pointer to it.
  CPDF_Object* AddIndirectObject(std::unique_ptr<CPDF_Object> pObj);

  // Always takes ownership of |pObj|, return true if higher generation number.
  bool ReplaceIndirectObjectIfHigherGeneration(
      uint32_t objnum,
      std::unique_ptr<CPDF_Object> pObj);

  uint32_t GetLastObjNum() const { return m_LastObjNum; }
  void SetLastObjNum(uint32_t objnum) { m_LastObjNum = objnum; }

  const_iterator begin() const { return m_IndirectObjs.begin(); }
  const_iterator end() const { return m_IndirectObjs.end(); }

 protected:
  virtual std::unique_ptr<CPDF_Object> ParseIndirectObject(uint32_t objnum);

 private:
  uint32_t m_LastObjNum;
  std::map<uint32_t, std::unique_ptr<CPDF_Object>> m_IndirectObjs;
};

#endif  // CORE_FPDFAPI_PARSER_CPDF_INDIRECT_OBJECT_HOLDER_H_
